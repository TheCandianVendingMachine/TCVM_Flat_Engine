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

#include "feAssert.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <fstream>

fe::engine *fe::engine::m_instance = nullptr;

void fe::engine::run()
    {
        std::ofstream out("profileOutput.txt");
        fe::clock updateClock;
        float currentTime = updateClock.getTime().asSeconds();
        float newTime = 0.f;
        float frameTime = 0.f;

        int framesPassed = 0;
        int inverseDeltaTime = 1 / m_deltaTime;
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

                FE_ENGINE_PROFILE("engine", "update")
                update();
                FE_END_PROFILE;

                FE_ENGINE_PROFILE("engine", "draw")
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
        FE_ENGINE_PROFILE("engine", "state_preupdate");
        m_gameStateMachine->preUpdate();
        FE_END_PROFILE;
        FE_ENGINE_PROFILE("engine", "state_update");
        m_gameStateMachine->update(m_collisionWorld);
        FE_END_PROFILE;

        unsigned int iterations = m_accumulator / m_deltaTime;
        FE_ENGINE_PROFILE("engine", "physics_preupdate");
        m_physicsEngine->preUpdate(m_deltaTime, iterations);
        FE_END_PROFILE;

        int iterationTest = 0;
        FE_ENGINE_PROFILE("engine", "fixed_timestep");
        while (m_accumulator >= m_deltaTime)
            {
                m_inputManager->handleKeyPress();
                m_accumulator -= m_deltaTime;
                ++iterationTest;
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine", "physics_timestep_sim");
        m_physicsEngine->simulateForces(m_deltaTime, iterations);
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine", "state_postupdate");
        m_gameStateMachine->postUpdate();
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine", "collision_world_collide");
        m_collisionWorld->handleCollisions(m_gameStateMachine->getDynamicBroadphase(), m_gameStateMachine->getStaticBroadphase());
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine", "send_events");
        m_eventSender->sendEvents();
        FE_END_PROFILE;
    }

void fe::engine::draw()
    {
        FE_ENGINE_PROFILE("engine", "pre_draw")
        m_gameStateMachine->preDraw();
        FE_END_PROFILE

        FE_ENGINE_PROFILE("engine", "window_clear")
        m_renderer.getRenderWindow().clear(sf::Color::Black);
        FE_END_PROFILE

        FE_ENGINE_PROFILE("engine", "window_buf1_draw");
        m_gameStateMachine->draw(m_renderer.getRenderWindow());
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine", "debug_buf1_draw");
        m_debugDraw->draw(m_renderer.getRenderWindow());
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine", "window_buf2_draw")
        m_renderer.getRenderWindow().display();
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("engine", "post_draw")
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
                m_memoryManager.startUp(totalMemory, stackMemory, dynamicMemory);

                m_logger = new(m_memoryManager.alloc(sizeof(fe::logger))) logger;
                m_logger->startUp("log.log");

                m_profileLogger = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::profilerLogger))) fe::profilerLogger();
                m_profileLogger->startUp();

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

                m_threadPool = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::threadPool<8>))) fe::threadPool<8>();
                m_threadPool->startUp();

                m_random.startUp();

                m_localization = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::localizationStorage))) fe::localizationStorage();
                m_prefabGuiElements = new(m_memoryManager.getStackAllocater().alloc(sizeof(fe::guiPrefabricatedElements))) fe::guiPrefabricatedElements();

                m_instance = this;
            }
    }

void fe::engine::shutDown()
    {
        m_shutDown = true;

        m_threadPool->shutDown();

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

const fe::Vector2d fe::engine::getWindowSize() const
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

fe::engine::~engine()
    {
        if (!m_shutDown)
            {
                //m_memoryManager.printDebugInformation();
            }
    }