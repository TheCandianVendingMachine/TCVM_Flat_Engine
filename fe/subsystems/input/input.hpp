// input.hpp
// A struct that has data that defines inputs
#pragma once
#include <SFML/Window/Event.hpp>
#include <type_traits>
#include <functional>

namespace fe
    {
        // a base class so we can store any type of input as a pointer
        struct inputBase
            {
                inputBase(bool realTime, bool inverse) : m_realTime(realTime), m_inverse(inverse), m_frozen(false) {}

                virtual void handleEvent(const sf::Event &eve) {}
                virtual void checkPressed() {}

                bool m_realTime;
                bool m_inverse;
                bool m_frozen;
            };

        template<typename TInput>
        struct input : public inputBase
            {
                static constexpr sf::Event::EventType m_release = (std::is_same<TInput, sf::Keyboard::Key>::value) ? sf::Event::KeyReleased : sf::Event::MouseButtonReleased;
                static constexpr sf::Event::EventType m_pressed = (std::is_same<TInput, sf::Keyboard::Key>::value) ? sf::Event::KeyPressed : sf::Event::MouseButtonPressed;

                static const bool m_isKeyboard = std::is_same<TInput, sf::Keyboard::Key>::value;
                
                TInput m_input;

                std::function<void()> m_callback;

                input() : inputBase(false, false) {}
                input(bool realTime, bool onPress, TInput input, std::function<void()> callback) : inputBase(realTime, !onPress), m_input(input), m_callback(callback) { }

                input &operator=(const input &rhs)
                    {
                        if (this != &rhs)
                            {
                                m_callback = rhs.m_callback;
                                m_input = rhs.m_input;
                                m_realTime = rhs.m_realTime;
                                m_inverse = rhs.m_inverse;
                            }

                        return *this;
                    }

                void handleEvent(const sf::Event &event)
                    {
                        if (m_frozen) return;

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
                        if (m_frozen) return;

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