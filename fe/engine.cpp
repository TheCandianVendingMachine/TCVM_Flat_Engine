#include "engine.hpp"
#include <SFML/Window/Event.hpp>



void fe::engine::handleEvents()
    {
        sf::Event currentEvent;
        while (m_renderWindow->pollEvent(currentEvent))
            {
                if (currentEvent.type == sf::Event::Closed)
                    {
                        m_renderWindow->close();
                    }

                m_inputManager->handleEvents(currentEvent);
            }
    }

void fe::engine::update()
    {
        while (m_accumulator >= m_deltaTime)
            {
                m_accumulator -= m_deltaTime;
            }
    }

void fe::engine::draw()
    {
        m_renderWindow->clear(sf::Color::Black);

        m_renderWindow->display();
    }

fe::engine::engine(const float updateRate) :
    m_logger(nullptr),
    m_deltaTime(updateRate)
    {}

void fe::engine::loadWindow()
    {
        m_renderWindow->create(sf::VideoMode(1280, 720), "Flat Engine", sf::Style::Close);
        m_renderWindow->setFramerateLimit(2000);
    }

void fe::engine::startUp(unsigned long long totalMemory, unsigned long long stackMemory)
    {
        m_memoryManager.startUp(totalMemory, stackMemory);


        m_logger = new logger;
        m_logger->startUp("log.log");

        m_renderWindow = new sf::RenderWindow;
        loadWindow();

        m_inputManager = new inputManager;
        m_inputManager->startUp();
    }

void fe::engine::shutDown()
    {
        m_inputManager->shutDown();
        m_logger->shutDown();
        m_memoryManager.shutDown();
    }

void fe::engine::run()
    {
        fe::clock updateClock;
        float currentTime = updateClock.getTime().asSeconds();

        while (m_renderWindow->isOpen())
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
