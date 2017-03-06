#include "clock.hpp"
#include <chrono>
#include <iomanip>

fe::time fe::clock::m_getCurrentTime()
    {
        auto currentTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
        return fe::microseconds(currentTime.time_since_epoch().count());
    }

fe::clock::clock()
    {
        restart();
        m_stopped = false;
    }

void fe::clock::restart()
    {
        m_startTime = m_getCurrentTime();
    }

void fe::clock::stop(bool value)
    {
        fe::time currentTime = m_getCurrentTime();
        m_stopped = value;

        // if the clock is restarted we have to subtract the time the clock has been non-active from start_time
        if (!m_stopped)
            {
                m_offsetTime += m_stopTime - m_startTime;
            }

        m_stopTime = currentTime;
    }

fe::time fe::clock::getTime()
    {
        if (!m_stopped)
            {
                return m_getCurrentTime() - m_startTime - m_offsetTime;
            }

        return m_stopTime - m_startTime;
    }

fe::time fe::clock::getTimeSinceEpoch()
    {
        auto currentTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
        return fe::microseconds(currentTime.time_since_epoch().count());
    }

const char *fe::clock::getFormattedTime(const char *format)
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto timeInfo = std::localtime(&now);

        static char lastTimeAsked[500];
        strftime(lastTimeAsked, 25, format, timeInfo);

        return lastTimeAsked;
    }
