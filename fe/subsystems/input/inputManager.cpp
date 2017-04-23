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
        for (auto &input : m_inputs)
            {
                if (input.second)
                    {
                        delete input.second;
                    }
            }

        m_inputs.clear();
    }

void fe::inputManager::handleEvents(const sf::Event &event)
    {
        for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it)
            {
                if (it->second)
                    {
                        it->second->handleEvent(event);
                    }
            }
    }

void fe::inputManager::handleKeyPress()
    {
        for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it)
            {
                if (it->second)
                    {
                        it->second->checkPressed();
                    }
            }
    }

fe::inputManager &fe::inputManager::get()
    {
        return *m_instance;
    }

void fe::inputManager::add(const char *id, input<sf::Keyboard::Key> input)
    {
        m_inputs[id] = new fe::input<sf::Keyboard::Key>(input);
    }

void fe::inputManager::add(const char *id, input<sf::Mouse::Button> input)
    {
        m_inputs[id] = new fe::input<sf::Mouse::Button>(input);
    }

void fe::inputManager::remove(const char *id)
    {
        m_inputs.erase(id);
    }

void fe::inputManager::setActive(const char *id, bool value)
    {
        m_inputs[id]->m_frozen = !value;
    }
