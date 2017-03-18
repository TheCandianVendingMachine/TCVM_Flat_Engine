#include "engine.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

float fe::engine::m_deltaTimeStatic = 0.f;

void fe::engine::handleEvents()
    {
        sf::Event currentEvent;
        while (m_renderer.getRenderWindow().pollEvent(currentEvent))
            {
                if (currentEvent.type == sf::Event::Closed)
                    {
                        m_renderer.getRenderWindow().close();
                    }

                m_inputManager->handleEvents(currentEvent);
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

fe::engine::engine(const float updateRate) :
    m_logger(nullptr),
    m_deltaTime(updateRate)
    {
        m_deltaTimeStatic = updateRate;
    }

void fe::engine::startUp(unsigned long long totalMemory, unsigned long long stackMemory)
    { 
        m_memoryManager.startUp(totalMemory, stackMemory);
 
        m_logger = new logger;
        m_logger->startUp("log.log");

        m_renderer.startUp();
        m_renderer.load();

        m_inputManager = new inputManager;
        m_inputManager->startUp();

        m_gameStateMachine = new gameStateMachine;
        m_gameStateMachine->startUp();
    }

void fe::engine::shutDown()
    {
        // We manually call the destructors because we want them to go out of scope now

        m_gameStateMachine->shutDown();
        m_gameStateMachine->~gameStateMachine();

        m_inputManager->shutDown();
        m_inputManager->~inputManager();

        m_renderer.shutDown();
        m_renderer.~renderer();

        m_logger->shutDown();
        m_logger->~logger();

        m_memoryManager.shutDown();
        m_memoryManager.~memoryManager();
    }

void fe::engine::run()
    {
        fe::clock updateClock;
        float currentTime = updateClock.getTime().asSeconds();

        while (m_renderer.getRenderWindow().isOpen())
            {
                float newTime = updateClock.getTime().asSeconds();
                float frameTime = newTime - currentTime;

                if (frameTime > 0.25f) frameTime = 0.25f;
                currentTime = newTime;

                m_accumulator += frameTime;

                handleEvents();
                update();
                draw();
            }
    }

fe::renderer *fe::engine::getRenderer()
	{
		return &m_renderer;
	}

const float fe::engine::getDeltaTime()
    {
        return m_deltaTimeStatic;
    }

void fe::engine::queueState(baseGameState *state)
    {
        m_gameStateMachine->queuePush(state);
    }
void fe::engine::queuePop()
    {
        m_gameStateMachine->queuePop();
    }