// input.hpp
// A struct that has data that defines inputs
#pragma once
#include <SFML/Window/Event.hpp>
#include <type_traits>
#include "../../misc/function.hpp"

namespace fe
    {
        // Really bad work around to be able to store inputs that may have a class member as a callback
        struct inputBase 
            {
                virtual void handleEvent(const sf::Event &eve) {}
                virtual void checkPressed() {}
            };

        template<typename TInput, typename Object = void>
        struct input : public inputBase
            {
                static constexpr sf::Event::EventType m_release = (std::is_same<TInput, sf::Keyboard::Key>::value) ? sf::Event::KeyReleased : sf::Event::MouseButtonReleased;
                static constexpr sf::Event::EventType m_pressed = (std::is_same<TInput, sf::Keyboard::Key>::value) ? sf::Event::KeyPressed : sf::Event::MouseButtonPressed;

                static const bool m_isKeyboard = std::is_same<TInput, sf::Keyboard::Key>::value;
                
                TInput m_input;

                bool m_realTime;
                bool m_inverse;

                fe::function<void(Object)> m_callback;

                input() {}
                input(bool realTime, bool onPress, TInput input, fe::function<void(Object)> callback) : m_input(input), m_callback(callback), m_realTime(realTime), m_inverse(!onPress) { }
                input(bool realTime, bool onPress, TInput input, void(*callback)()) : m_input(input), m_callback(callback), m_realTime(realTime), m_inverse(!onPress) { }

                void handleEvent(const sf::Event &event)
                    {
                        bool correctKey = m_isKeyboard ? m_input == event.key.code : m_input == event.mouseButton.button;
                        bool eventPressed = event.type == m_pressed && correctKey;

                        if (m_callback && !m_realTime && correctKey &&
                            ((eventPressed && !m_inverse) ||
                            (!eventPressed && m_inverse)))
                            {
                                m_callback();
                            }
                    }

                void checkPressed()
                    {
                        // static_cast is required. The compiler doesn't like mixing/matching types, even if it cannot happen, so we
                        // cast it to the type it arleady is
                        bool isPressed = (std::is_same<TInput, sf::Keyboard::Key>::value ?
                                          sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(m_input)) :
                                          sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(m_input)));

                        if (m_callback && m_realTime &&
                            ((isPressed && !m_inverse) ||
                            (!isPressed && m_inverse)))
                            {
                                m_callback();
                            }
                    }
            };
    }