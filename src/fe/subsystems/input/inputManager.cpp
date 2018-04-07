#include "fe/subsystems/input/inputManager.hpp"
#include "fe/debug/logger.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>

fe::inputManager *fe::inputManager::m_instance = nullptr;

bool fe::inputManager::correctInputEvent(sf::Keyboard::Key key, const sf::Event &event)
    {
        return event.key.code == key;
    }

bool fe::inputManager::correctInputEvent(sf::Mouse::Button button, const sf::Event &event)
    {
        return event.mouseButton.button == button;
    }

bool fe::inputManager::correctInputEvent(sf::Mouse::Wheel wheel, const sf::Event &event)
    {
        return true;
    }

bool fe::inputManager::correctInputEvent(sf::Keyboard::Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

bool fe::inputManager::correctInputEvent(sf::Mouse::Button button)
    {
        return sf::Mouse::isButtonPressed(button);
    }

sf::Event::EventType fe::inputManager::getEvent(input *input, sf::Keyboard::Key key)
    {
        if (input->m_realTime)
            {
                return sf::Event::EventType::Count;
            }

        if (input->m_inverse)
            {
                return sf::Event::EventType::KeyReleased;
            }

        return sf::Event::EventType::KeyPressed;
    }

sf::Event::EventType fe::inputManager::getEvent(input *input, sf::Mouse::Button button)
    {
        if (input->m_realTime)
            {
                return sf::Event::EventType::Count;
            }

        if (input->m_inverse)
            {
                return sf::Event::EventType::MouseButtonReleased;
            }

        return sf::Event::EventType::MouseButtonPressed;
    }

sf::Event::EventType fe::inputManager::getEvent(input *input, sf::Mouse::Wheel wheel)
    {
        if (input->m_realTime)
            {
                return sf::Event::EventType::Count;
            }

        return sf::Event::EventType::MouseWheelScrolled;
    }

void fe::inputManager::setActive(fe::input *input, bool keyIsGood, bool active)
    {
        /*
            Truth Table:
            ________________________________________
            | keyIsGood | active | frozen | result |
            | 0         | 0      | 0      | 0      | -> (!keyIsGood & frozen)
            | 0         | 0      | 1      | 1      | -> (!keyIsgood & frozen)
            | 0         | 1      | 0      | 0      | -> (!keyIsGood & frozen)
            | 0         | 1      | 1      | 1      | -> (!keyIsGood & frozen)
            | 1         | 0      | 0      | 0      | -> (keyIsGood & active)
            | 1         | 0      | 1      | 0      | -> (keyIsGood & active)
            | 1         | 1      | 0      | 1      | -> (keyIsGood & active)
            | 1         | 1      | 1      | 1      | -> (keyIsGood & active)
            ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
            Therefore: (!keyIsGood & frozen) || (keyIsGood & active)
            (!active) because of the inverse needed to determine frozen from active
        */
        input->m_frozen = (!keyIsGood & input->m_frozen) || (keyIsGood & !active);
    }

void fe::inputManager::startUp()
    {
        if (!m_instance)
            {
                m_instance = this;
            }
    }

void fe::inputManager::shutDown()
    {}

void fe::inputManager::preUpdate()
    {
        
    }

void fe::inputManager::handleEvents(const sf::Event &event)
    {
        for (auto &inputHandle : m_eventInputs[event.type])
            {
                auto input = getObject(inputHandle);
                if (!input->m_frozen)
                    {
                        bool correctKey = false;
                        switch (input->m_alias.m_inputType)
                            {
                                case fe::imp::inputAlias::KEY:
                                    correctKey = correctInputEvent(input->m_alias.m_input.m_key, event);
                                    break;
                                case fe::imp::inputAlias::BUTTON:
                                    correctKey = correctInputEvent(input->m_alias.m_input.m_button, event);
                                    break;
                                case fe::imp::inputAlias::WHEEL:
                                    correctKey = correctInputEvent(input->m_alias.m_input.m_wheel, event);
                                    break;
                                default:
                                    break;
                            }

                        if (correctKey) 
                            {
                                input->m_callback(event);
                            }
                    }
            }

        switch (event.type)
            {
                case sf::Event::KeyPressed:
                    m_realtimeKeyPressed[event.key.code] = true;
                    break;
                case sf::Event::KeyReleased:
                    m_realtimeKeyPressed[event.key.code] = false;
                    break;
                default:
                    break;
            }
    }

void fe::inputManager::handleKeyPress()
    {
        for (auto &inputHandle : m_realtimeInputs)
            {
                auto input = getObject(inputHandle);
                if (!input->m_frozen)
                    {
                        bool correctKey = false;
                        switch (input->m_alias.m_inputType)
                            {
                                case fe::imp::inputAlias::KEY:
                                    correctKey = correctInputEvent(input->m_alias.m_input.m_key);
                                    break;
                                case fe::imp::inputAlias::BUTTON:
                                    correctKey = correctInputEvent(input->m_alias.m_input.m_button);
                                    break;
                                default:
                                    break;
                            }

                        if (correctKey) 
                            {
                                input->m_callback(sf::Event());
                            }
                    }
            }
    }

fe::inputManager &fe::inputManager::get()
    {
        return *m_instance;
    }

fe::Handle fe::inputManager::add(sf::Keyboard::Key key, input data)
    {
        auto handle = addObject(new input(data));
        getObject(handle)->m_alias = imp::inputAlias(key);
        auto event = getEvent(getObject(handle), key);
        if (event != sf::Event::Count)
            {
                m_eventInputs[event].push_back(handle);
            }
        else
            {
                m_realtimeInputs.push_back(handle);
            }
        return handle;
    }

fe::Handle fe::inputManager::add(sf::Mouse::Button button, input data)
    {
        auto handle = addObject(new input(data));
        getObject(handle)->m_alias = imp::inputAlias(button);
        auto event = getEvent(getObject(handle), button);
        if (event != sf::Event::Count)
            {
                m_eventInputs[event].push_back(handle);
            }
        else
            {
                m_realtimeInputs.push_back(handle);
            }
        return handle;
    }

fe::Handle fe::inputManager::add(sf::Mouse::Wheel wheel, input data)
    {
        auto handle = addObject(new input(data));
        getObject(handle)->m_alias = imp::inputAlias(wheel);
        auto event = getEvent(getObject(handle), wheel);
        if (event != sf::Event::Count)
            {
                m_eventInputs[event].push_back(handle);
            }
        else
            {
                m_realtimeInputs.push_back(handle);
            }
        return handle;
    }

void fe::inputManager::remove(fe::Handle handle)
    {
        removeObject(handle);

        m_realtimeInputs.erase(std::find(m_realtimeInputs.begin(), m_realtimeInputs.end(), handle), m_realtimeInputs.end());
        for (auto &eventInput : m_eventInputs)
            {
                eventInput.second.erase(std::find(eventInput.second.begin(), eventInput.second.end(), handle), eventInput.second.end());
            }
    }

void fe::inputManager::setActive(sf::Keyboard::Key key, bool active)
    {
        for (auto &inputHandle : m_realtimeInputs)
            {
                auto input = getObject(inputHandle);
                switch (input->m_alias.m_inputType)
                    {
                        case fe::imp::inputAlias::KEY:
                            setActive(input, input->m_alias.m_input.m_key == key, active);
                            break;
                        default:
                            break;
                    }
            }

        for (auto &inputHandle : m_eventInputs[sf::Event::KeyPressed])
            {
                auto input = getObject(inputHandle);
                setActive(input, input->m_alias.m_input.m_key == key, active);
            }

        for (auto &inputHandle : m_eventInputs[sf::Event::KeyReleased])
            {
                auto input = getObject(inputHandle);
                setActive(input, input->m_alias.m_input.m_key == key, active);
            }
    }

void fe::inputManager::setActive(sf::Mouse::Button button, bool active)
    {
        for (auto &inputHandle : m_realtimeInputs)
            {
                auto input = getObject(inputHandle);
                switch (input->m_alias.m_inputType)
                    {
                        case fe::imp::inputAlias::BUTTON:
                            setActive(input, input->m_alias.m_input.m_button == button, active);
                            break;
                        default:
                            break;
                    }
            }

        for (auto &inputHandle : m_eventInputs[sf::Event::MouseButtonPressed])
            {
                auto input = getObject(inputHandle);
                setActive(input, input->m_alias.m_input.m_button == button, active);
            }

        for (auto &inputHandle : m_eventInputs[sf::Event::MouseButtonReleased])
            {
                auto input = getObject(inputHandle);
                setActive(input, input->m_alias.m_input.m_button == button, active);
            }
    }

void fe::inputManager::setActive(sf::Mouse::Wheel wheel, bool active)
    {
        for (auto &inputHandle : m_eventInputs[sf::Event::MouseWheelMoved])
            {
                auto input = getObject(inputHandle);
                setActive(input, input->m_alias.m_input.m_wheel == wheel, active);
            }

        for (auto &inputHandle : m_eventInputs[sf::Event::EventType::MouseWheelScrolled])
            {
                auto input = getObject(inputHandle);
                setActive(input, input->m_alias.m_input.m_wheel == wheel, active);
            }
    }

void fe::inputManager::setActive(fe::Handle handle, bool active)
    {
        getObject(handle)->m_frozen = !active;
    }

bool fe::inputManager::isKeyPressed(sf::Keyboard::Key key) const
    {
        if (m_realtimeKeyPressed.find(key) != m_realtimeKeyPressed.end())
            {
                return m_realtimeKeyPressed.find(key)->second;
            }
        return false;
    }
