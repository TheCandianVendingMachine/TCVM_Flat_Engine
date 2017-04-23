#include "inputManager.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

fe::inputManager *fe::inputManager::m_instance = nullptr;

void fe::inputManager::startUp()
    {
        if (!m_instance)
            {
                m_instance = this;
            }
    }

void fe::inputManager::shutDown()
    {
        m_keyboardInputs.clear();
        m_mouseInputs.clear();
    }

void fe::inputManager::handleEvents(const sf::Event &event)
    {
        for (auto &input : m_keyboardInputs)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == input.first)
                    {
                        for (auto &data : input.second)
                            {
                                if (!data.m_realTime && !data.m_frozen)
                                    {
                                        data.m_callback();
                                    }
                            }
                    }
            }

        for (auto &input : m_mouseInputs)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == input.first)
                    {
                        for (auto &data : input.second)
                            {
                                if (!data.m_realTime && !data.m_frozen)
                                    {
                                        data.m_callback();
                                    }
                            }
                    }
            }
    }

void fe::inputManager::handleKeyPress()
    {
        for (auto &input : m_keyboardInputs)
            {
                if (sf::Keyboard::isKeyPressed(input.first))
                    {
                        for (auto &data : input.second)
                            {
                                if (data.m_realTime && !data.m_frozen)
                                    {
                                        data.m_callback();
                                    }
                            }
                    }
            }

        for (auto &input : m_mouseInputs)
            {
                if (sf::Mouse::isButtonPressed(input.first))
                    {
                        for (auto &data : input.second)
                            {
                                if (data.m_realTime && !data.m_frozen)
                                    {
                                        data.m_callback();
                                    }
                            }
                    }
            }
    }

fe::inputManager &fe::inputManager::get()
    {
        return *m_instance;
    }

void fe::inputManager::add(sf::Keyboard::Key key, input data)
    {
        m_keyboardInputs[key].push_back(data);
    }

void fe::inputManager::add(sf::Mouse::Button key, input data)
    {
        m_mouseInputs[key].push_back(data);
    }

void fe::inputManager::setActive(sf::Keyboard::Key key, bool active)
    {
        for (auto &input : m_keyboardInputs[key])
            {
                input.m_frozen = !active;
            }
    }

void fe::inputManager::setActive(sf::Mouse::Button key, bool active)
    {
        for (auto &input : m_mouseInputs[key])
            {
                input.m_frozen = !active;
            }
    }
