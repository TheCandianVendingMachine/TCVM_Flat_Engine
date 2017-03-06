// input.hpp
// A struct that has data that defines inputs
#pragma once
#include <SFML/Window/Event.hpp>
#include <type_traits>

namespace fe
    {
        template<typename TInput>
        struct input
            {
                static constexpr sf::Event::EventType m_release = (std::is_same<TInput, sf::Keyboard::Key>::value) ? sf::Event::KeyReleased : sf::Event::MouseButtonReleased;
                static constexpr sf::Event::EventType m_pressed = (std::is_same<TInput, sf::Keyboard::Key>::value) ? sf::Event::KeyPressed : sf::Event::MouseButtonPressed;
                
                TInput m_input;
                bool m_eventPressed;

                input() {}
                input(TInput input) : m_input(input) { }
                void handleEvent(const sf::Event &event)
                    {
                        m_eventPressed = event.type == m_pressed;
                    }

            };
    }