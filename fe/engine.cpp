#include "engine.hpp"
#include "subsystems/gameState/gameStateMachine.hpp"
#include "subsystems/messaging/eventSender.hpp"
#include "subsystems/input/inputManager.hpp"
#include "debug/logger.hpp"

#include "feAssert.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

fe::engine *fe::engine::m_instance = nullptr;

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
        m_gameStateMachine->preUpdate();
        while (m_accumulator >= m_deltaTime)
            {
                m_inputManager->handleKeyPress();
                m_gameStateMachine->update(m_deltaTime);
                m_accumulator -= m_deltaTime;
            }

        m_eventSender->sendEvents();
        m_gameStateMachine->postUpdate();
    }

void fe::engine::draw()
    {
        m_gameStateMachine->preDraw();
        m_renderer.getRenderWindow().clear(sf::Color::Black);
        m_gameStateMachine->draw(m_renderer.getRenderWindow());
        m_renderer.getRenderWindow().display();

        m_gameStateMachine->postDraw();
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

                m_renderer.startUp();
                m_renderer.load();

                m_inputManager = new inputManager;
                m_inputManager->startUp();

                m_eventSender = new fe::eventSender;

                m_gameStateMachine = new gameStateMachine;
                m_gameStateMachine->startUp();

                m_screenSize = m_renderer.getWindowSize();

                m_instance = this;
            }
    }

void fe::engine::shutDown()
    {
        m_shutDown = true;

        m_gameStateMachine->shutDown();
        m_inputManager->shutDown();
        m_renderer.shutDown();
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

void fe::engine::run()
    {
        fe::clock updateClock;
        float currentTime = updateClock.getTime().asSeconds();

        int framesPassed = 0;
        while (m_renderer.getRenderWindow().isOpen())
            {
                float newTime = updateClock.getTime().asSeconds();
                float frameTime = newTime - currentTime;

                if (frameTime > 0.25f) frameTime = 0.25f;
                currentTime = newTime;

                m_accumulator += frameTime;

                m_inputManager->preUpdate();
                handleEvents();
                update();
                draw();

                calcFPS();
            }
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

const fe::renderer &fe::engine::getRenderer() const
    {
        return m_renderer;
    }

fe::eventSender *fe::engine::getEventSender() const
    {
        return m_eventSender;
    }

fe::engine::~engine()
    {
        if (!m_shutDown)
            {
                //m_memoryManager.printDebugInformation();
            }
    }
