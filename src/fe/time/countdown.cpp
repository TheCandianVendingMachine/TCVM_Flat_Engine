#include "fe/time/countdown.hpp"

fe::countdown::countdown() : m_started(false)
    {
    }

void fe::countdown::pause(bool pause)
    {
        m_timer.stop(pause);
    }

void fe::countdown::start(fe::time length, fe::time startTime)
    {
        m_length = length + startTime;
        m_started = true;
        m_timer.restart();
    }

void fe::countdown::reset()
    {
        m_started = false;
    }

bool fe::countdown::isDone(fe::time currentTime) const
    {
        return m_started && hasCompleted(currentTime);
    }

bool fe::countdown::hasCompleted(fe::time currentTime) const
    {
        return timeUntilCompletion(currentTime).asMicroseconds() <= 0;
    }

fe::time fe::countdown::timeUntilCompletion(fe::time currentTime) const
    {
        return m_length - currentTime;
    }

bool fe::countdown::hasStarted() const
    {
        return m_started;
    }

fe::time fe::countdown::getInternalClockTime() const
    {
        return m_timer.getTime();
    }


