#include "inputManager.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

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
        for (auto &key : m_keyboardInputs)
            {
                key.second.handleEvent(event);
            }

        for (auto &button : m_mouseInputs)
            {
                button.second.handleEvent(event);
            }
    }

fe::inputManager &fe::inputManager::get()
    {
        return *m_instance;
    }

void fe::inputManager::add(const std::string &id, input<sf::Keyboard::Key> input)
    {
        m_keyboardInputs[id] = input;
    }

void fe::inputManager::add(const std::string &id, input<sf::Mouse::Button> input)
    {
        m_mouseInputs[id] = input;
    }

bool fe::inputManager::isKeyPressed(const std::string &id)
    {
        return sf::Keyboard::isKeyPressed(m_keyboardInputs[id].m_input);
    }

bool fe::inputManager::isButtonPressed(const std::string &id)
    {
        return sf::Mouse::isButtonPressed(m_mouseInputs[id].m_input);
    }

bool fe::inputManager::keyEventPress(const std::string &id)
    {
        return m_keyboardInputs[id].m_eventPressed;
    }

bool fe::inputManager::buttonEventPress(const std::string &id)
    {
        return m_mouseInputs[id].m_eventPressed;
    }
