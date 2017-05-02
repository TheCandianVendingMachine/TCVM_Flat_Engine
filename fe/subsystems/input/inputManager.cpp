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

void fe::inputManager::handleEvents(const sf::Event &event)
    {
        for (auto &input : m_keyboardInputs)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == input.first)
                    {
                        for (auto &data : input.second)
                            {
                                if (!(*data)->m_realTime && !(*data)->m_frozen)
                                    {
                                        (*data)->m_callback();
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
                                if (!(*data)->m_realTime && !(*data)->m_frozen)
                                    {
                                        (*data)->m_callback();
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
                                if ((*data)->m_realTime && !(*data)->m_frozen)
                                    {
                                        (*data)->m_callback();
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
                                if ((*data)->m_realTime && !(*data)->m_frozen)
                                    {
                                        (*data)->m_callback();
                                    }
                            }
                    }
            }
    }

fe::inputManager &fe::inputManager::get()
    {
        return *m_instance;
    }

unsigned int fe::inputManager::add(sf::Keyboard::Key key, input data)
    {
        m_inputs.push_back(new input(data));
        m_keyboardInputs[key].push_back(&m_inputs.back());
        m_handles.push_back(m_inputs.size() - 1);

        return m_handles.back();
    }

unsigned int fe::inputManager::add(sf::Mouse::Button key, input data)
    {
        m_inputs.push_back(new input(data));
        m_mouseInputs[key].push_back(&m_inputs.back());
        m_handles.push_back(m_inputs.size() - 1);

        return m_handles.back();
    }

void fe::inputManager::remove(unsigned int handle)
    {
        if (m_handles.begin() + handle < m_handles.end())
            {
                (*(m_inputs.begin() + m_handles[handle])) = nullptr;
                m_inputs.erase(m_inputs.begin() + m_handles[handle]);
                // since we are erasing an entity, all handles above it will become invalid. To prevent this, we will subtract all handles
                // above and including the current one by one.
                for (auto it = m_handles.begin() + handle; it != m_handles.end(); ++it) { (*it) -= 1; }

                for (auto &inp : m_keyboardInputs)
                    {
                        inp.second.erase(std::remove_if(inp.second.begin(), inp.second.end(), [](input **inp){ return inp == nullptr; }), inp.second.end());
                    }

                for (auto &inp : m_mouseInputs)
                    {
                        inp.second.erase(std::remove_if(inp.second.begin(), inp.second.end(), [](input **inp){ return inp == nullptr; }), inp.second.end());
                    }
            }
        else
            {
                FE_LOG_WARNING("Cannot remove input with handle \"", handle, "\"");
            }
    }

void fe::inputManager::setActive(sf::Keyboard::Key key, bool active)
    {
        for (auto &input : m_keyboardInputs[key])
            {
                (*input)->m_frozen = !active;
            }
    }

void fe::inputManager::setActive(sf::Mouse::Button key, bool active)
    {
        for (auto &input : m_mouseInputs[key])
            {
                (*input)->m_frozen = !active;
            }
    }

void fe::inputManager::setActive(unsigned int handle, bool active)
    {
        if (m_handles.begin() + handle < m_handles.end())
            {
                m_inputs[m_handles[handle]]->m_frozen = !active;
            }
        else
            {
                FE_LOG_WARNING("Cannot disable input with handle \"", handle, "\"");
            }
    }
