#include "engine.hpp"
#include "subsystems/gameState/gameStateMachine.hpp"
#include "subsystems/messaging/eventSender.hpp"
#include "subsystems/input/inputManager.hpp"
#include "subsystems/resourceManager/resourceManager.hpp"
#include "subsystems/physics/physicsEngine.hpp"
#include "subsystems/physics/collision/collisionWorld.hpp"
#include "subsystems/threading/threadPool.hpp"
#include "subsystems/threading/threadJob.hpp"
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

                FE_PROFILE("engine", "frame");
                FE_PROFILE("engine", "input_preUpdate")
                m_inputManager->preUpdate();
                FE_END_PROFILE;

                FE_PROFILE("engine", "event");
                handleEvents();
                FE_END_PROFILE;

                FE_PROFILE("engine", "update")
                update();
                FE_END_PROFILE;

                FE_PROFILE("engine", "draw")
                draw();
                FE_END_PROFILE;
                FE_END_PROFILE;

            #if FE_PROFILE_ENGINE
                if (m_elapsedFrames % inverseDeltaTime == 0) 
                    {
                        std::ofstream out("profileOutput.txt");
                        m_profileLogger->printToStream(out);
                        out.flush();
                        out.close();
                    }
            #endif

                m_profileLogger->clearTotalCalls();
                calcFPS();
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
        FE_PROFILE("engine", "state_preupdate");
        m_gameStateMachine->preUpdate();
        FE_END_PROFILE;
        FE_PROFILE("engine", "state_update");
        m_gameStateMachine->update();
        FE_END_PROFILE;

        unsigned int iterations = m_accumulator / m_deltaTime;
        FE_PROFILE("engine", "physics_preupdate");
        m_physicsEngine->preUpdate(m_deltaTime, iterations);
        FE_END_PROFILE;

        int iterationTest = 0;
        FE_PROFILE("engine", "fixed_timestep");
        while (m_accumulator >= m_deltaTime)
            {
                m_inputManager->handleKeyPress();
                m_accumulator -= m_deltaTime;
                ++iterationTest;
            }
        FE_END_PROFILE;

        FE_PROFILE("engine", "physics_timestep_sim");
        m_physicsEngine->simulateForces(m_deltaTime, iterations);
        FE_END_PROFILE;

        FE_PROFILE("engine", "collision_world_update_threaded");
        m_collisionWorld->update();
        FE_END_PROFILE;

        FE_PROFILE("engine", "state_postupdate");
        m_gameStateMachine->postUpdate();
        FE_END_PROFILE;

        FE_PROFILE("engine", "collision_world_collide");
        m_collisionWorld->handleCollisions();
        FE_END_PROFILE;

        FE_PROFILE("engine", "send_events");
        m_eventSender->sendEvents();
        FE_END_PROFILE;
    }

void fe::engine::draw()
    {
        FE_PROFILE("engine", "pre_draw")
        m_gameStateMachine->preDraw();
        FE_END_PROFILE

        FE_PROFILE("engine", "window_clear")
        m_renderer.getRenderWindow().clear(sf::Color::Black);
        FE_END_PROFILE

        FE_PROFILE("engine", "window_buf1_draw");
        m_gameStateMachine->draw(m_renderer.getRenderWindow());
        FE_END_PROFILE;

        FE_PROFILE("engine", "debug_buf1_draw");
        m_debugDraw->draw(m_renderer.getRenderWindow());
        FE_END_PROFILE;

        FE_PROFILE("engine", "window_buf2_draw")
        m_renderer.getRenderWindow().display();
        FE_END_PROFILE;

        FE_PROFILE("engine", "post_draw")
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

void fe::engine::startUp(unsigned long long totalMemory, unsigned long long stackMemory)
    { 
        FE_ASSERT((m_instance == nullptr), "Engine instance already created!");

        if (!m_instance)
            {
                m_memoryManager.startUp(totalMemory, stackMemory);

                m_logger = new(m_memoryManager.alloc(sizeof(fe::logger))) logger;
                m_logger->startUp("log.log");

                m_profileLogger = new fe::profilerLogger();
                m_profileLogger->startUp();

                m_debugDraw = new fe::debugDraw();
                m_debugDraw->startUp();

                m_renderer.startUp();
                m_renderer.load();

                m_inputManager = new inputManager;
                m_inputManager->startUp();

                m_eventSender = new fe::eventSender;

                m_gameStateMachine = new gameStateMachine;
                m_gameStateMachine->startUp();

                m_physicsEngine = new physicsEngine;
                m_physicsEngine->startUp();

                m_collisionWorld = new collisionWorld;
                m_collisionWorld->startUp();

                m_fontManager = new resourceManager<sf::Font>;
                m_textureManager = new resourceManager<sf::Texture>;

                m_screenSize = m_renderer.getWindowSize();

                m_threadPool = new fe::threadPool<8>();
                m_threadPool->startUp();

                m_random.startUp();

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

const fe::baseGameState &fe::engine::getCurrentState() const
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

fe::eventSender *fe::engine::getEventSender() const
    {
        return m_eventSender;
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

fe::engine::~engine()
    {
        if (!m_shutDown)
            {
                //m_memoryManager.printDebugInformation();
            }
    }
