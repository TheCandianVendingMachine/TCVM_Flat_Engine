// input.hpp
// A struct that has data that defines inputs
#pragma once
#include <SFML/Window/Event.hpp>
#include <functional>
#include <string>

namespace fe
    {
        struct input
            {
                std::function<void()> m_callback;
                sf::Event m_event; // if it is triggered by an event or not, this is valid
                bool m_realTime; // trigged by event or not

                bool m_frozen;

                input() : m_event(sf::Event()), m_realTime(false), m_frozen(false) {}
                input(std::function<void()> callback, bool realTime, sf::Event event = sf::Event()) : m_callback(callback), m_realTime(realTime), m_event(event), m_frozen(false) {}
            };
    }