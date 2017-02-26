#include "countdown.hpp"

void fe::countdown::start(fe::time length)
    {
        m_length = length;
        m_timer.restart();
    }

bool fe::countdown::isDone()
    {
        return timeUntilCompletion().asMicroseconds() <= 0;
    }

fe::time fe::countdown::timeUntilCompletion()
    {
        return m_length - m_timer.getTime();
    }
