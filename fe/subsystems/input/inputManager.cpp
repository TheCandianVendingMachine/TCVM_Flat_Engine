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
        
    }

void fe::inputManager::handleEvents(const sf::Event &event)
    {
        for (auto it = m_keyboardInputs.begin(); it != m_keyboardInputs.end(); ++it)
            {
                it->handleEvent(event);
            }

        for (auto &button : m_mouseInputs)
            {
                button.handleEvent(event);
            }
    }

void fe::inputManager::handleKeyPress()
    {
        for (auto it = m_keyboardInputs.begin(); it != m_keyboardInputs.end(); ++it)
            {
                it->checkPressed();
            }

        for (auto &button : m_mouseInputs)
            {
                button.checkPressed();
            }
    }

fe::inputManager &fe::inputManager::get()
    {
        return *m_instance;
    }

void fe::inputManager::add(input<sf::Keyboard::Key> input)
    {
        m_keyboardInputs.push_back(input);
    }

void fe::inputManager::add(input<sf::Mouse::Button> input)
    {
        m_mouseInputs.push_back(input);
    }
