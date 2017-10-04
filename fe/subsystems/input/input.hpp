// input.hpp
// A struct that has data that defines inputs
#pragma once
#include <SFML/Window/Event.hpp>
#include <functional>

namespace fe
    {
        struct input
            {
                std::function<void()> m_callback;
                bool m_realTime; // trigged by event or not

                bool m_frozen;
                bool m_inverse;

                input() : m_realTime(false), m_frozen(false), m_inverse(false) {}
                input(std::function<void()> callback, bool realTime, bool inverse) : m_callback(callback), m_realTime(realTime), m_inverse(inverse), m_frozen(false) {}
            };
    }