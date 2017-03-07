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

                if (currentEvent.type == sf::Event::KeyReleased)
                    {
                        if (currentEvent.key.code == sf::Keyboard::T)
                            {
                                m_deltaTime = 1.f/ 144.f;
                                std::cout << "SwitchA\n";
                            }
                        else if (currentEvent.key.code == sf::Keyboard::Y)
                            {
                                m_deltaTime = 1.f / 60.f;
                                std::cout << "SwitchB\n";
                            }
                        else if (currentEvent.key.code == sf::Keyboard::U)
                            {
                                m_deltaTime = 1.f / 30.f;
                                std::cout << "SwitchC\n";
                            }
                    }
            }
    }

void fe::engine::update()
    {
        while (m_accumulator >= m_deltaTime)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) velocity.y = -200.f;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) velocity.y = 200.f;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x = -200.f;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x = 200.f;

                test->move(velocity.convertToSfVec2() * m_deltaTime);
                m_accumulator -= m_deltaTime;
            }
    }

void fe::engine::draw()
    {
        m_renderWindow->clear(sf::Color::Black);
        m_renderWindow->draw(*test);
        m_renderWindow->display();
    }

fe::engine::engine(const float updateRate) :
    m_logger(nullptr),
    m_deltaTime(updateRate)
    {}

void fe::engine::loadWindow()
    {
        m_renderWindow->create(sf::VideoMode(720, 480), "Flat Engine", sf::Style::Close);
        m_renderWindow->setFramerateLimit(100);
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

        fe::inputManager::get().add("up", sf::Keyboard::W);
        fe::inputManager::get().add("left", sf::Keyboard::A);
        fe::inputManager::get().add("down", sf::Keyboard::S);
        fe::inputManager::get().add("right", sf::Keyboard::D);
    }

void fe::engine::shutDown()
    {
        m_logger->shutDown();
        m_memoryManager.shutDown();
        m_inputManager->shutDown();
    }

void fe::engine::run()
    {
        sf::RectangleShape dog(fe::Vector2d(50, 50).convertToSfVec2());
        test = &dog;

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

                velocity = fe::Vector2d();
                
                update();
                draw();
            }
    }
