#include "fe/time/countdown.hpp"

fe::countdown::countdown() : m_started(false)
    {
    }

void fe::countdown::pause(bool pause)
	{
		m_timer.stop(pause);
	}

void fe::countdown::start(fe::time length)
    {
        m_length = length;
        m_started = true;
        m_timer.restart();
    }

bool fe::countdown::isDone()
    {
        if (m_started && hasCompleted())
            {
                m_started = false;
                return true;
            }

        return false;
    }

bool fe::countdown::hasCompleted()
	{
		return timeUntilCompletion().asMicroseconds() <= 0;
	}

bool fe::countdown::hasStarted()
    {
        return m_started;
    }

fe::time fe::countdown::timeUntilCompletion()
    {
        return m_length - m_timer.getTime();
    }
