#include "inputManager.hpp"
#include "../../debug/logger.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>

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

void fe::inputManager::preUpdate()
    {
        for (auto &input : m_keyboardInputs)
            {
                input.second.erase(std::remove_if(input.second.begin(), input.second.end(), [this](Handle handle) { return !handleActive(handle); } ), input.second.end());
            }

        for (auto &input : m_mouseInputs)
            {
                input.second.erase(std::remove_if(input.second.begin(), input.second.end(), [this](Handle handle) { return !handleActive(handle); }), input.second.end());
            }
    }

void fe::inputManager::handleEvents(const sf::Event &event)
    {
        for (auto &inputData : m_keyboardInputs)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == inputData.first)
                    {
                        for (auto &data : inputData.second)
                            {
                                auto input = getObject(data);
                                if (input)
                                    {
                                        if (!input->m_realTime && !input->m_frozen)
                                            {
                                                input->m_callback();
                                            }
                                    }
                            }
                    }
            }

        for (auto &inputData : m_mouseInputs)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == inputData.first)
                    {
                        for (auto &data : inputData.second)
                            {
                                auto input = getObject(data);
                                if (input)
                                    {
                                        if (!input->m_realTime && !input->m_frozen)
                                            {
                                                input->m_callback();
                                            }
                                    }
                            }
                    }
            }
    }

void fe::inputManager::handleKeyPress()
    {
        for (auto &inputData : m_keyboardInputs)
            {
                if (sf::Keyboard::isKeyPressed(inputData.first))
                    {
                        for (auto &data : inputData.second)
                            {
                                auto input = getObject(data);
                                if (input)
                                    {
                                        if (input->m_realTime && !input->m_frozen)
                                            {
                                                input->m_callback();
                                            }
                                    }
                            }
                    }
            }

        for (auto &inputData : m_mouseInputs)
            {
                if (sf::Mouse::isButtonPressed(inputData.first))
                    {
                        for (auto &data : inputData.second)
                            {
                                auto input = getObject(data);
                                if (input)
                                    {
                                        if (input->m_realTime && !input->m_frozen)
                                            {
                                                input->m_callback();
                                            }
                                    }
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
        m_keyboardInputs[key].push_back(handle);

        return handle;
    }

fe::Handle fe::inputManager::add(sf::Mouse::Button key, input data)
    {
        auto handle = addObject(new input(data));
        m_mouseInputs[key].push_back(handle);

        return handle;
    }

void fe::inputManager::setActive(sf::Keyboard::Key key, bool active)
    {
        for (auto &input : m_keyboardInputs[key])
            {
                setActive(input, active);
            }
    }

void fe::inputManager::setActive(sf::Mouse::Button key, bool active)
    {
        for (auto &input : m_mouseInputs[key])
            {
                setActive(input, active);
            }
    }

void fe::inputManager::setActive(fe::Handle handle, bool active)
    {
        getObject(handle)->m_frozen = !active;
    }
