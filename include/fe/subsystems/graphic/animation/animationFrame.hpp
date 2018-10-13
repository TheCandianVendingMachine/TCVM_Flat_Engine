// animationFrame.hpp
// defines the animation frame in terms of {frame coord, time}
#pragma once
#include "fe/math/Vector2.hpp"
#include "fe/time/time.hpp"

namespace fe
    {
        struct animationFrame
            {
                fe::Vector2<unsigned int> m_frame;
                fe::time m_time;

                animationFrame() {}
                animationFrame(fe::Vector2<unsigned int> frame, fe::time time) : m_frame(frame), m_time(time) {}
            };
    }

