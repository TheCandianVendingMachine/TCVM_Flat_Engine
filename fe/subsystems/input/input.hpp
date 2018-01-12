// input.hpp
// A struct that has data that defines inputs
#pragma once
#include <SFML/Window/Event.hpp>
#include <functional>

namespace sf
    {
        struct Event;
    }

namespace fe
    {
        namespace imp
            {
                struct inputAlias
                    {
                        union inputButton
                            {
                                sf::Keyboard::Key m_key;
                                sf::Mouse::Button m_button;
                                sf::Mouse::Wheel m_wheel;

                                inputButton() {}
                                inputButton(sf::Keyboard::Key key) : m_key(key) {}
                                inputButton(sf::Mouse::Button button) : m_button(button) {}
                                inputButton(sf::Mouse::Wheel wheel) : m_wheel(wheel) {}
                            };

                        inputButton m_input;

                        enum
                            {
                                NONE,
                                KEY,
                                BUTTON,
                                WHEEL
                            } m_inputType;

                        inputAlias() {}
                        inputAlias(sf::Keyboard::Key key) : m_input(key), m_inputType(KEY) {}
                        inputAlias(sf::Mouse::Button button) : m_input(button), m_inputType(BUTTON) {}
                        inputAlias(sf::Mouse::Wheel wheel) : m_input(wheel), m_inputType(WHEEL) {}

                    };
            }

        struct input
            {
                std::function<void(const sf::Event&)> m_callback;
                imp::inputAlias m_alias;
                bool m_realTime; // trigged by event or not

                bool m_frozen;
                bool m_inverse;

                input() : m_realTime(false), m_frozen(false), m_inverse(false), m_alias() {}
                input(std::function<void(const sf::Event&)> callback, bool realTime, bool inverse) : m_callback(callback), m_realTime(realTime), m_inverse(inverse), m_frozen(false) {}

            };
    }