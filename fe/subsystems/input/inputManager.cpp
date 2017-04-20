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
        for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it)
            {
                it->second->handleEvent(event);
            }
    }

void fe::inputManager::handleKeyPress()
    {
        for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it)
            {
                it->second->checkPressed();
            }
    }

fe::inputManager &fe::inputManager::get()
    {
        return *m_instance;
    }

void fe::inputManager::add(const char *id, input<sf::Keyboard::Key> input)
    {
        void *mem = FE_ALLOC_STACK("InputManager", sizeof(fe::input<sf::Keyboard::Key>));
        fe::input<sf::Keyboard::Key> *inputAlloc = new(mem) fe::input<sf::Keyboard::Key>;

        *inputAlloc = input;
        m_inputs[id] = inputAlloc;
    }

void fe::inputManager::add(const char *id, input<sf::Mouse::Button> input)
    {
        void *mem = FE_ALLOC_STACK("InputManager", sizeof(fe::input<sf::Mouse::Button>));
        fe::input<sf::Mouse::Button> *inputAlloc = new(mem) fe::input<sf::Mouse::Button>;

        *inputAlloc = input;
        m_inputs[id] = inputAlloc;
    }

void fe::inputManager::setActive(const char *id, bool value)
    {
        m_inputs[id]->m_frozen = !value;
    }
