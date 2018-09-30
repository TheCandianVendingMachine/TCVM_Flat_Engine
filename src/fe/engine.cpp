#include "fe/engine.hpp"
#include "fe/subsystems/gameState/gameStateMachine.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/subsystems/messaging/gameEvent.hpp"
#include "fe/subsystems/input/inputManager.hpp"
#include "fe/subsystems/resourceManager/resourceManager.hpp"
#include "fe/subsystems/physics/physicsEngine.hpp"
#include "fe/subsystems/collision/collisionWorld.hpp"
#include "fe/subsystems/threading/threadPool.hpp"
#include "fe/subsystems/threading/threadJob.hpp"
#include "fe/localization/localizationStorage.hpp"
#include "fe/debug/logger.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/debug/profilerLogger.hpp"
#include "fe/debug/debugDraw.hpp"
#include "fe/subsystems/filesystem/fileUtilities.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"
#include "fe/subsystems/scripting/commonScriptFunctions.hpp"
#include "fe/entity/scriptObject.hpp"
#include "fe/entity/component/component.hpp"
#include "fe/engineCompileHelper.hpp"

#include "fe/feAssert.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <filesystem>

fe::engine *fe::engine::m_instance = nullptr;

void fe::engine::run()
    {
        fe::clock updateClock;
        float currentTime = updateClock.getTime().asSeconds();
        float newTime = 0.f;
        float frameTime = 0.f;

        int framesPassed = 0;
        int inverseDeltaTime = static_cast<int>(1.f / m_deltaTime);
        while (m_renderer.getRenderWindow().isOpen())
            {
                newTime = updateClock.getTime().asSeconds();
                frameTime = newTime - currentTime;

                if (frameTime > 0.25f) frameTime = 0.25f;
                currentTime = newTime;

                m_accumulator += frameTime;

                FE_ENGINE_PROFILE("engine", "frame");
                FE_ENGINE_PROFILE("engine", "input_preUpdate")
                m_inputManager->preUpdate();
                FE_END_PROFILE;

                FE_ENGINE_PROFILE("engine", "event");
                handleEvents();
                FE_END_PROFILE;

                FE_ENGINE_PROFILE("engine_update", "update")
                update();
                FE_END_PROFILE;

                FE_ENGINE_PROFILE("engine_draw", "draw")
                draw();
                FE_END_PROFILE;
                FE_END_PROFILE;

            #if FE_OUTPUT_PROFILE_RESULTS
                if (m_elapsedFrames % inverseDeltaTime == 0) 
                    {
                        std::ofstream out("profileOutput.txt", std::ios::trunc);
                        m_profileLogger->printToStream(out);
                        out.close();
                        //FE_LOG_DEBUG(getFPS());
                    }
            #endif

                m_profileLogger->clearTotalCalls();
                calcFPS();

                m_scriptManager->update();

                m_logger->swapConsoleBuffer();
                m_logger->swapFileBuffer();

                m_elapsedGameTime = currentTime;
            }
    }

void fe::engine::handleEvents()
    {
        sf::Event currentEvent;
        while (m_renderer.getRenderWindow().pollEvent(currentEvent))
            {
                if (currentEvent.type == sf::Event::Closed)
                    {
                        m_renderer.getRenderWindow().close();
                    }
                else if (currentEvent.type == sf::Event::MouseMoved)
                    {
                        m_mousePosition = m_renderer.getRenderWindow().mapPixelToCoords({ currentEvent.mouseMove.x, currentEvent.mouseMove.y });
                    }

                m_inputManager->handleEvents(currentEvent);
                m_gameStateMachine->handleEvents(currentEvent);
            }
    }

void fe::engine::update()
    {
        FE_ENGINE_PROFILE("engine_update", "state_preupdate");
        m_gameStateMachine->preUpdate();
        FE_END_PROFILE;
        FE_ENGINE_PROFILE("engine_update", "state_update");
        m_gameStateMachine->update(m_collisionWorld);
        FE_END_PROFILE;

        float test = std::floorf(m_accumulator / m_deltaTime);
        unsigned int iterations = static_cast<unsigned int>(std::floorf(m_accumulator / m_deltaTime));
        FE_ENGINE_PROFILE("engine_update", "physics_preupdate");
        m_physicsEngine->preUpdate(m_deltaTime, iterations);
        FE_END_PROFILE;

        int iterationTest = 0;
        FE_ENGINE_PROFILE("engine_update", "fixed_timestep");
        while (m_accumulator >= m_deltaTime)
            {
                m_inputManager->handleKeyPress();
                m_gameStateMachine->fixedUpdate(m_deltaTime);
                m_accumulator -= m_deltaTime;
                ++iterationTest;
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_update", "physics_timestep_sim");
        m_physicsEngine->simulateForces(m_deltaTime, iterations);
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_update", "state_postupdate");
        m_gameStateMachine->postUpdate();
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_update", "camera_update");
        m_gameStateMachine->cameraUpdate(m_deltaTime, iterations);
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_update", "collision_world_collide");
        m_collisionWorld->handleCollisions(m_gameStateMachine->getDynamicBroadphase(), m_gameStateMachine->getStaticBroadphase());
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_update", "send_events");
        m_eventSender->sendEvents();
        FE_END_PROFILE;
    }

void fe::engine::draw()
    {
        FE_ENGINE_PROFILE("engine_draw", "pre_draw")
        m_gameStateMachine->preDraw();
        FE_END_PROFILE

        FE_ENGINE_PROFILE("engine_draw", "window_clear")
        m_renderer.getRenderWindow().clear(m_clearColour);
        FE_END_PROFILE

        FE_ENGINE_PROFILE("engine_draw", "window_buf1_draw");
        m_gameStateMachine->draw(m_renderer.getRenderWindow());
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_draw", "debug_buf1_draw");
        m_debugDraw->draw(m_renderer.getRenderWindow());
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_draw", "window_buf2_draw")
        m_renderer.getRenderWindow().display();
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine_draw", "post_draw")
        m_gameStateMachine->postDraw();
        FE_END_PROFILE;
    }

void fe::engine::calcFPS()
    {
        m_elapsedFrames++;
        if (m_fpsClock.getTime().asSeconds() > 1.f)
            {
                int timeElapsed = m_fpsClock.getTime().asMilliseconds();
                m_fps = 1000.f / ((float)timeElapsed / (float)m_elapsedFrames);
                m_elapsedFrames = 0;
                m_fpsClock.restart();
            }
    }

void fe::engine::registerLua()
    {
        // Initialize a basic entity types in Lua
        m_scriptManager->runLua(R"(squareEntity = {
    sceneGraph = {
        renderType = "renderObject",
        zPos = 0,
        colour = { r = 255, g = 255, b = 255, a = 255 }
    },
    size = { x = 0, y = 0 }
})");

        m_scriptManager->runLua(R"(error_func = function()
    return 0 
end)");

        fe::compileLuaKeybinds(m_scriptManager);

        m_scriptManager->getEnumHandler().registerEnum("gameEventArgumentType",
            "bool", fe::gameEventArgument::type::TYPE_BOOL,
            "float", fe::gameEventArgument::type::TYPE_FLOAT,
            "int", fe::gameEventArgument::type::TYPE_INT,
            "string", fe::gameEventArgument::type::TYPE_STRING,
            "uint", fe::gameEventArgument::type::TYPE_UINT,
            "voidp", fe::gameEventArgument::type::TYPE_VOIDP
        );

        // Register Functions
        m_scriptManager->getFunctionHandler().registerCPPFunction("isInputPressed", &fe::isInputPressed);
        m_scriptManager->getFunctionHandler().registerCPPFunction("setCameraPosition", &fe::setCameraPosition);
        m_scriptManager->getFunctionHandler().registerCPPFunction("breakToDebugger", &fe::breakToDebugger);
        m_scriptManager->getFunctionHandler().registerCPPFunction("sendEvent", &fe::sendEvent);

        // Register Objects
        m_scriptManager->getUserTypeHandler().addCustomType<fe::Vector2d, fe::Vector2d(), fe::Vector2d(float, float)>(
            "vector2",
            "x", &Vector2d::x,
            "y", &Vector2d::y
        );

        m_scriptManager->getUserTypeHandler().addCustomType<fe::lightVector2d, fe::lightVector2d(), fe::lightVector2d(float, float)>(
            "lightVector2",
            "x", &lightVector2d::x,
            "y", &lightVector2d::y
        );

        m_scriptManager->getUserTypeHandler().addCustomType<fe::gameEventArgument>(
            "gameEventArgument",
            "getType", &fe::gameEventArgument::argType,
            "argument", &fe::gameEventArgument::arg
        );

        m_scriptManager->getUserTypeHandler().addCustomType<scriptObject>(
            "scriptObject",
            "getName", &scriptObject::scriptObjectGetName,
            "setForce", &scriptObject::scriptObjectSetForce,
            "setVelocity", &scriptObject::scriptObjectSetVelocity,
            "applyForce", &scriptObject::scriptObjectApplyForce,
            "getPosition", &scriptObject::scriptObjectGetPosition,
            "destroy", &scriptObject::scriptObjectDestroy,
            "getNormalForce", &scriptObject::scriptObjectGetNormalForce,
            "getForce", &scriptObject::scriptObjectGetForce,
            "getVelocity", &scriptObject::scriptObjectGetVelocity,
            "getDirection", &scriptObject::scriptObjectGetDirection,
            "setColour", &scriptObject::scriptObjectSetColour,
            "moveToPosition", &scriptObject::scriptObjectMoveToPosition,
            "getSlowdownDistance", &scriptObject::scriptObjectGetSlowdownDistance,
            "setSlowdownDistance", &scriptObject::scriptObjectSetSlowdownDistance,
            "hasArrived", &scriptObject::scriptObjectHasArrived,
            "isTargeted", &scriptObject::scriptObjectIsTargeted
        );
    }

fe::engine::engine(const float updateRate) :
    m_logger(nullptr),
    m_deltaTime(updateRate),
    m_elapsedFrames(0),
    m_shutDown(false),
    m_clearColour(sf::Color::Black)
    {
        FE_ASSERT((m_instance == nullptr), "Engine instance already created!");
    }

void fe::engine::startUp(fe::uInt64 totalMemory, fe::uInt64 stackMemory, fe::uInt64 dynamicMemory)
    { 
        FE_ASSERT((m_instance == nullptr), "Engine instance already created!");

        if (!m_instance)
            {
                m_instance = this;

                m_memoryManager.startUp(totalMemory);
                m_memoryManager.startUpStack(stackMemory);

                m_threadPool = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::threadPool<8>))) fe::threadPool<8>();
                m_threadPool->startUp();

                m_logger = new(m_memoryManager.alloc(sizeof(fe::logger))) fe::logger;
                m_logger->startUp("log.log");

                m_profileLogger = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::profilerLogger))) fe::profilerLogger();
                m_profileLogger->startUp();

                m_memoryManager.startUpDynamic(dynamicMemory);

                m_debugDraw = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::debugDraw))) fe::debugDraw();
                m_debugDraw->startUp();

                m_renderer.startUp();
                m_renderer.load();

                m_inputManager = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::inputManager))) inputManager;
                m_inputManager->startUp();

                m_eventSender = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::eventSender))) fe::eventSender;

                m_gameStateMachine = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::gameStateMachine))) gameStateMachine;
                m_gameStateMachine->startUp();

                m_physicsEngine = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::physicsEngine))) physicsEngine;
                m_physicsEngine->startUp();

                m_collisionWorld = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::collisionWorld))) collisionWorld;
                m_collisionWorld->startUp();

                m_fontManager = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::resourceManager<sf::Font>))) resourceManager<sf::Font>;
                m_textureManager = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::resourceManager<sf::Texture>))) resourceManager<sf::Texture>;

                m_screenSize = m_renderer.getWindowSize();

                m_random.startUp();

                m_localization = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::localizationStorage))) fe::localizationStorage();

                m_scriptManager = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::scriptManager))) fe::scriptManager();
                m_scriptManager->startUp();

                m_defaultCamera.setSize(getWindowSize());
                m_defaultCamera.setPosition(getWindowSize() / 2.f);

                registerLua();
                
            }
    }

void fe::engine::shutDown()
    {
        m_shutDown = true;

        m_scriptManager->shutDown();

        m_fontManager->shutDown();
        m_textureManager->shutDown();

        m_collisionWorld->shutDown();
        m_physicsEngine->shutDown();
        m_gameStateMachine->shutDown();
        m_inputManager->shutDown();
        m_renderer.shutDown();
        m_debugDraw->shutDown();
        m_profileLogger->shutDown();

        m_logger->swapFileBuffer();
        m_logger->shutDown();
        m_logger->~logger();

        m_threadPool->shutDown();
        m_memoryManager.shutDown();
    }

void fe::engine::close() const
    {
        getRenderer().getRenderWindow().close();
    }

void fe::engine::crash(const char *reason)
    {
        FE_LOG_ERROR("Engine has crashed!");
        FE_LOG_ERROR(reason);
        m_instance->close();
        m_instance->shutDown();
        std::quick_exit(0);
    }

const fe::engine &fe::engine::get()
    {
        return *m_instance;
    }

const float fe::engine::getDeltaTime() const
    {
        return m_deltaTime;
    }

const float fe::engine::getFPS() const
    {
        return m_fps;
    }

fe::time fe::engine::getElapsedTime() const
    {
        return m_elapsedTime.getTime();
    }

fe::time fe::engine::getElapsedGameTime() const
    {
        return fe::seconds(m_elapsedGameTime);
    }

const fe::Vector2<unsigned int> fe::engine::getWindowSize() const
    {
        return m_screenSize;
    }

const fe::Vector2d fe::engine::getMousePos() const
    {
        return m_mousePosition;
    }

fe::gameStateMachine &fe::engine::getStateMachine() const
    {
        return *m_gameStateMachine;
    }

fe::baseGameState &fe::engine::getCurrentState() const
    {
        return m_gameStateMachine->getCurrentState();
    }

fe::inputManager &fe::engine::getInputManager() const
    {
        return *m_inputManager;
    }

const fe::renderer &fe::engine::getRenderer() const
    {
        return m_renderer;
    }

fe::eventSender &fe::engine::getEventSender() const
    {
        return *m_eventSender;
    }

fe::physicsEngine &fe::engine::getPhysicsEngine() const
    {
        return *m_physicsEngine;
    }

fe::collisionWorld &fe::engine::getCollisionWorld() const
    {
       return *m_collisionWorld;
    }

fe::threadPool<8> &fe::engine::getThreadPool() const
    {
        return *m_threadPool;
    }

fe::localizationStorage &fe::engine::getLocalization() const
    {
        return *m_localization;
    }

fe::guiPrefabricatedElements &fe::engine::getPrefabGui() const
    {
        return *m_prefabGuiElements;
    }

const fe::camera &fe::engine::getDefaultCamera() const
    {
        return m_defaultCamera;
    }

fe::scriptManager &fe::engine::getScriptManager() const
    {
        return *m_scriptManager;
    }

void fe::engine::loadResources(const char *resourcesFile)
    {
        enum resourceTypes
            {
                ERROR_TYPE,
                TEXTURE,
                AUDIO,
                FONT,
                SCRIPT
            };

        std::ifstream in(resourcesFile);
        std::string line;
        std::unordered_map<std::string, std::string> resources;

        std::vector<std::string> resourceFilePaths;
        std::vector<std::string> resourceSynonymPaths;
        {
            enum
                {
                    READING,
                    RESOURCE_FILE_PATH,
                    RESOURCE_SYNONYM,
                    RESOURCE,
                    RESOURCE_NAME,
                } currentState;
        
            currentState = READING;

            while (std::getline(in, line))
                {
                    std::string data0 = "";
                    std::string data1 = "";
                    for (unsigned int i = 0; i < line.size(); i++)
                        {
                            char c = line[i];
                            if (c >= '!')
                                {
                                    switch (currentState)
                                        {
                                            case READING:
                                                data0 = "";
                                                data1 = "";
                                                if (c == '[')
                                                    {
                                                        currentState = RESOURCE_FILE_PATH;
                                                    }
                                                else if (c == '{')
                                                    {
                                                        currentState = RESOURCE_SYNONYM;
                                                    }
                                                else
                                                    {
                                                        currentState = RESOURCE;
                                                        data0 += c;
                                                    }
                                                break;
                                            case RESOURCE_FILE_PATH:
                                                if (c == ']')
                                                    {
                                                        resourceFilePaths.push_back(data0);
                                                        currentState = READING;
                                                    }
                                                else
                                                    {
                                                        data0 += c;
                                                    }
                                                break;
                                            case RESOURCE_SYNONYM:
                                                if (c == '}')
                                                    {
                                                        resourceSynonymPaths.push_back(data0);
                                                        currentState = READING;
                                                    }
                                                else
                                                    {
                                                        data0 += c;
                                                    }
                                                break;
                                            case RESOURCE:
                                                if (c == '=')
                                                    {
                                                        currentState = RESOURCE_NAME;
                                                    }
                                                else
                                                    {
                                                        data0 += c;
                                                    }
                                                break;
                                            case RESOURCE_NAME:
                                                if (c == ';' || i == line.size() - 1)
                                                    {
                                                        if (i == line.size() - 1)
                                                            {
                                                                data1 += c;
                                                            }
                                                        resources[data0] = data1;
                                                        currentState = READING;
                                                    }
                                                else
                                                    {
                                                        data1 += c;
                                                    }
                                                break;
                                            default:
                                                break;
                                        }
                                }
                        }
                }
            in.close();
        }

        std::unordered_map<std::string, resourceTypes> synonymTypes;
        std::vector<std::string> extensions;
        {
            enum
                {
                    READING_TYPE,
                    READING_TYPE_NAME,
                    READING_EXTENSION,
                } currentState;
            currentState = READING_TYPE;

            for (auto &synonymFile : resourceSynonymPaths) 
                {
                    in.open(synonymFile);
                    std::string extension;
                    std::string type;
                    while (std::getline(in, line))
                        {
                            int index = 0;
                            bool readingName = false;
                            for (unsigned int i = 0; i < line.size(); i++)
                                {
                                    char c = line[i];
                                    switch (currentState)
                                        {
                                            case READING_TYPE:
                                                if (std::tolower(c) == "start"[index++])
                                                    {
                                                        if (index == std::strlen("start"))
                                                            {
                                                                currentState = READING_TYPE_NAME;
                                                                index = 0;
                                                                break;
                                                            }
                                                    }
                                                else
                                                    {
                                                        index = 0;
                                                    }
                                                break;
                                            case READING_TYPE_NAME:
                                                if (c == '\n' || i == line.size() - 1)
                                                    {
                                                        currentState = READING_EXTENSION;
                                                        index = 0;
                                                    }
                                                
                                                if (c >= '!')
                                                    {
                                                        type += std::tolower(c);
                                                    }
                                                break;
                                            case READING_EXTENSION:
                                                if (std::tolower(c) == "end"[index++])
                                                    {
                                                        if (index == std::strlen("end"))
                                                            {
                                                                currentState = READING_TYPE;
                                                                index = 0;
                                                                type = "";
                                                                extension = "";
                                                                break;
                                                            }
                                                    }
                                                else
                                                    {
                                                        index = 0;
                                                    }

                                                if (c >= '!')
                                                    {
                                                        extension += c;
                                                    }

                                                if (c == '\n' || i == line.size() - 1)
                                                    {
                                                        resourceTypes typeEnum = ERROR_TYPE;
                                                        if (type == "texture")
                                                            {
                                                                typeEnum = TEXTURE;
                                                            }
                                                        else if (type == "audio")
                                                            {
                                                                typeEnum = AUDIO;
                                                            }
                                                        else if (type == "font")
                                                            {
                                                                typeEnum = FONT;
                                                            }
                                                        else if (type == "script")
                                                            {
                                                                typeEnum = SCRIPT;
                                                            }

                                                        synonymTypes[extension] = typeEnum;
                                                        extensions.push_back(extension);
                                                        extension = "";
                                                    }
                                                break;
                                            default:
                                                break;
                                        }
                                }
                        }
                    in.close();
                }
        }

        std::string currentPath = std::experimental::filesystem::current_path().string();
        std::vector<std::string> allFiles;
        for (auto &file : resourceFilePaths)
            {
                fe::getAllFilesInDirectory((currentPath + "/" + file), std::move(extensions), allFiles);
            }

        for (auto &possibleResource : allFiles)
            {
                std::string file = fe::getFileFromDirectory(possibleResource);
                if (synonymTypes[fe::getFileExtension(possibleResource)] == SCRIPT)
                    {
                        getScriptManager().runFile(possibleResource);
                    }

                if (resources.find(file) != resources.end())
                    {
                        switch (synonymTypes[fe::getFileExtension(possibleResource)])
                            {
                                case ERROR_TYPE:
                                    FE_LOG_WARNING("\\" + file + "\"is not an acceptable file type");
                                    break;
                                case TEXTURE:
                                    getResourceManager<sf::Texture>()->load(possibleResource.c_str(), resources[file].c_str());
                                    break;
                                case AUDIO:
                                    break;
                                case FONT:
                                    getResourceManager<sf::Font>()->load(possibleResource.c_str(), resources[file].c_str());
                                    break;
                                default:
                                    break;
                            }
                    }
            }
    }

void fe::engine::setClearColour(sf::Color colour)
    {
        m_clearColour = colour;
    }

fe::engine::~engine()
    {
        if (!m_shutDown)
            {
                //m_memoryManager.printDebugInformation();
            }
    }