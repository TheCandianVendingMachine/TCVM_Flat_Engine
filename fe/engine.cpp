#include "engine.hpp"
#include "subsystems/gameState/gameStateMachine.hpp"
#include "subsystems/messaging/eventSender.hpp"
#include "subsystems/input/inputManager.hpp"
#include "subsystems/resourceManager/resourceManager.hpp"
#include "subsystems/physics/physicsEngine.hpp"
#include "subsystems/physics/collision/collisionWorld.hpp"
#include "subsystems/threading/threadPool.hpp"
#include "subsystems/threading/threadJob.hpp"
#include "localization/localizationStorage.hpp"
#include "gui/guiPrefabricatedElements.hpp"
#include "debug/logger.hpp"
#include "debug/profiler.hpp"
#include "debug/profilerLogger.hpp"
#include "debug/debugDraw.hpp"
#include "subsystems/filesystem/fileUtilities.hpp"
#include "subsystems/scripting/scriptManager.hpp"
#include "subsystems/scripting/commonScriptFunctions.hpp"
#include "entity/scriptObject.hpp"

#include "feAssert.hpp"

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
        std::ofstream out("profileOutput.txt");
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
                        out.open("profileOutput.txt", std::ios::trunc);
                        m_profileLogger->printToStream(out);
                        out.close();
                        //FE_LOG_DEBUG(getFPS());
                    }
            #endif

                m_profileLogger->clearTotalCalls();
                calcFPS();

                m_logger->swapConsoleBuffer();
                m_logger->swapFileBuffer();

                m_elapsedTime = currentTime;
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

        unsigned int iterations = static_cast<unsigned int>(std::ceilf(m_accumulator / m_deltaTime));
        FE_ENGINE_PROFILE("engine_update", "physics_preupdate");
        m_physicsEngine->preUpdate(m_deltaTime, iterations);
        FE_END_PROFILE;

        int iterationTest = 0;
        FE_ENGINE_PROFILE("engine_update", "fixed_timestep");
        while (m_accumulator >= m_deltaTime)
            {
                m_inputManager->handleKeyPress();
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
        m_renderer.getRenderWindow().clear(sf::Color::Black);
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

        // Register C++ Enums
        m_scriptManager->getEnumHandler().registerEnum("inputs",
            "A",            sf::Keyboard::A,
            "B",            sf::Keyboard::B,
            "C",            sf::Keyboard::C,
            "D",            sf::Keyboard::D,
            "E",            sf::Keyboard::E,
            "F",            sf::Keyboard::F,
            "G",            sf::Keyboard::G,
            "H",            sf::Keyboard::H,
            "I",            sf::Keyboard::I,
            "J",            sf::Keyboard::J,
            "K",            sf::Keyboard::K,
            "L",            sf::Keyboard::L,
            "M",            sf::Keyboard::M,
            "N",            sf::Keyboard::N,
            "O",            sf::Keyboard::O,
            "P",            sf::Keyboard::P,
            "Q",            sf::Keyboard::Q,
            "R",            sf::Keyboard::R,
            "S",            sf::Keyboard::S,
            "T",            sf::Keyboard::T,
            "U",            sf::Keyboard::U,
            "V",            sf::Keyboard::V,
            "W",            sf::Keyboard::W,
            "X",            sf::Keyboard::X,
            "Y",            sf::Keyboard::Y,
            "Z",            sf::Keyboard::Z,
            "0",            sf::Keyboard::Num0,
            "1",            sf::Keyboard::Num1,
            "2",            sf::Keyboard::Num2,
            "3",            sf::Keyboard::Num3,
            "4",            sf::Keyboard::Num4,
            "5",            sf::Keyboard::Num5,
            "6",            sf::Keyboard::Num6,
            "7",            sf::Keyboard::Num7,
            "8",            sf::Keyboard::Num8,
            "9",            sf::Keyboard::Num9,
            "Escape",       sf::Keyboard::Escape,
            "LCtrl",        sf::Keyboard::LControl,
            "LShift",       sf::Keyboard::LShift,
            "LAlt",         sf::Keyboard::LAlt,
            "LSystem",      sf::Keyboard::LSystem,
            "RCtrl",        sf::Keyboard::RControl,
            "RShift",       sf::Keyboard::RShift,
            "RAlt",         sf::Keyboard::RAlt,
            "RSystem",      sf::Keyboard::RSystem,
            "Menu",         sf::Keyboard::Menu,
            "LBracket",     sf::Keyboard::LBracket,
            "RBracket",     sf::Keyboard::RBracket,
            "Semicolon",    sf::Keyboard::SemiColon,
            "Comma",        sf::Keyboard::Comma,
            "Period",       sf::Keyboard::Period,
            "Quote",        sf::Keyboard::Quote,
            "Slash",        sf::Keyboard::Slash,
            "Backslash",    sf::Keyboard::BackSlash,
            "Tilde",        sf::Keyboard::Tilde,
            "Equal",        sf::Keyboard::Equal,
            "Hyphen",       sf::Keyboard::Dash,
            "Space",        sf::Keyboard::Space,
            "Enter",        sf::Keyboard::Return,
            "Backspace",    sf::Keyboard::BackSpace,
            "Tab",          sf::Keyboard::Tab,
            "PageUp",       sf::Keyboard::PageUp,
            "PageDown",     sf::Keyboard::PageDown,
            "End",          sf::Keyboard::End,
            "Home",         sf::Keyboard::Home,
            "Insert",       sf::Keyboard::Insert,
            "Delete",       sf::Keyboard::Delete,
            "NumPlus",      sf::Keyboard::Add,
            "NumDash",      sf::Keyboard::Subtract,
            "NumStar",      sf::Keyboard::Multiply,
            "NumSlash",     sf::Keyboard::Divide,
            "Left",         sf::Keyboard::Left,
            "Right",        sf::Keyboard::Right,
            "Up",           sf::Keyboard::Up,
            "Down",         sf::Keyboard::Down,
            "Num0",         sf::Keyboard::Numpad0,
            "Num1",         sf::Keyboard::Numpad1,
            "Num2",         sf::Keyboard::Numpad2,
            "Num3",         sf::Keyboard::Numpad3,
            "Num4",         sf::Keyboard::Numpad4,
            "Num5",         sf::Keyboard::Numpad5,
            "Num6",         sf::Keyboard::Numpad6,
            "Num7",         sf::Keyboard::Numpad7,
            "Num8",         sf::Keyboard::Numpad8,
            "Num9",         sf::Keyboard::Numpad9,
            "F1",           sf::Keyboard::F1,
            "F2",           sf::Keyboard::F2,
            "F3",           sf::Keyboard::F3,
            "F4",           sf::Keyboard::F4,
            "F5",           sf::Keyboard::F5,
            "F6",           sf::Keyboard::F6,
            "F7",           sf::Keyboard::F7,
            "F8",           sf::Keyboard::F8,
            "F9",           sf::Keyboard::F9,
            "F10",          sf::Keyboard::F10,
            "F11",          sf::Keyboard::F11,
            "F12",          sf::Keyboard::F12,
            "F13",          sf::Keyboard::F13,
            "F14",          sf::Keyboard::F14,
            "F15",          sf::Keyboard::F15,
            "Pause",        sf::Keyboard::Pause,
            "LeftMouse",    sf::Mouse::Left + sf::Keyboard::KeyCount,
            "RightMouse",   sf::Mouse::Right + sf::Keyboard::KeyCount,
            "MiddleMouse",  sf::Mouse::Middle + sf::Keyboard::KeyCount,
            "XButton1",     sf::Mouse::XButton1 + sf::Keyboard::KeyCount,
            "XButton2",     sf::Mouse::XButton2 + sf::Keyboard::KeyCount
        );

        // Register Functions
        m_scriptManager->getFunctionHandler().registerFunction("isInputPressed", &fe::isInputPressed);

        // Register Objects
        m_scriptManager->getUserTypeHandler().addCustomType<scriptObject>("scriptObject", "applyForce", &scriptObject::applyForce);
    }

fe::engine::engine(const float updateRate) :
    m_logger(nullptr),
    m_deltaTime(updateRate),
    m_elapsedFrames(0),
    m_shutDown(false)
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
                m_prefabGuiElements = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::guiPrefabricatedElements))) fe::guiPrefabricatedElements();

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
        m_logger->shutDown();
        m_logger->~logger();
        m_threadPool->shutDown();
        m_memoryManager.shutDown();
    }

void fe::engine::close() const
    {
        getRenderer().getRenderWindow().close();
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
        return fe::seconds(m_elapsedTime);
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

fe::engine::~engine()
    {
        if (!m_shutDown)
            {
                //m_memoryManager.printDebugInformation();
            }
    }