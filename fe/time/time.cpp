#include "time.hpp"

fe::time::time()
    {
        m_time = 0;
    }

float fe::time::asSeconds()
    {
        // 0.000001 = 1e-6
        return static_cast<float>(static_cast<long double>(m_time) * 0.000001);
    }

int32_t fe::time::asMilliseconds()
    {
        return int32_t(static_cast<long double>(m_time) * 0.001);
    }

int64_t fe::time::asMicroseconds()
    {
        return int64_t(m_time);
    }

fe::time fe::time::operator+(const fe::time &rhs)
    {
        return fe::time(fe::microseconds(m_time + rhs.m_time));
    }

fe::time fe::time::operator-(const fe::time &rhs)
    {
        return fe::time(fe::microseconds(m_time - rhs.m_time));
    }

void fe::time::operator+=(const fe::time &rhs)
    {
        m_time += rhs.m_time;
    }

void fe::time::operator-=(const fe::time &rhs)
    {
        m_time -= rhs.m_time;
    }

fe::time &fe::time::operator=(const fe::time &rhs)
    {
        if (this != &rhs)
            {
                m_time = rhs.m_time;
            }
        return *this;
    }

bool fe::time::operator==(const fe::time &rhs)
    {
        return m_time = rhs.m_time;
    }


fe::time fe::seconds(const long double in)
    {
        fe::time ret;
        ret.m_time = static_cast<long long>(in / 0.000001);
        return ret;
    }

fe::time fe::milliseconds(const long double in)
    {
        fe::time ret;
        ret.m_time = static_cast<long long>(in / 0.001);
        return ret;
    }

fe::time fe::microseconds(const long double in)
    {
        fe::time ret;
        ret.m_time = in;
        return ret;
    }

fe::time fe::zero()
    {
        return fe::time();
    }
