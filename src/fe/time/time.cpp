#include "time.hpp"
#include <utility>

fe::time::time()
    {
        m_time = 0;
    }

fe::time::time(long long time)
    {
        m_time = time;
    }

fe::time::time(const fe::time &time)
    {
        if (&time != this)
            {
                m_time = time.m_time;
            }
    }

fe::time::time(const fe::time &&time)
    {
        m_time = std::forward<const long long>(time.m_time);
    }

double fe::time::asSeconds()
    {
        // 0.000001 = 1e-6
        return static_cast<double>(m_time) / 1000000;
    }

int32_t fe::time::asMilliseconds()
    {
        return static_cast<int32_t>(m_time) / 1000;
    }

int64_t fe::time::asMicroseconds()
    {
        return m_time;
    }

fe::time fe::time::operator+(const fe::time &rhs)
    {
        return fe::time(fe::microseconds(static_cast<long double>(m_time + rhs.m_time)));
    }

fe::time fe::time::operator-(const fe::time &rhs)
    {
        return fe::time(fe::microseconds(static_cast<long double>(m_time - rhs.m_time)));
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

fe::time fe::time::operator/(unsigned int rhs)
    {
        return fe::time(m_time / rhs);
    }

void fe::time::operator/=(unsigned int rhs)
    {
        m_time /= rhs;
    }

bool fe::time::operator>(const fe::time &rhs)
    {
        return m_time > rhs.m_time;
    }

bool fe::time::operator>=(const fe::time &rhs)
    {
        return m_time >= rhs.m_time;
    }

bool fe::time::operator<(const fe::time &rhs)
    {
        return m_time < rhs.m_time;
    }

bool fe::time::operator<=(const fe::time &rhs)
    {
        return m_time <= rhs.m_time;
    }

bool fe::time::operator==(const fe::time &rhs)
    {
        return m_time == rhs.m_time;
    }


fe::time fe::seconds(const long long in)
    {
        return fe::time(static_cast<float>(in / 1000000));
    }

fe::time fe::milliseconds(const long long in)
    {
        return fe::time(static_cast<int64_t>(in / 1000));
    }

fe::time fe::microseconds(const long long in)
    {
        return fe::time(static_cast<int64_t>(in));
    }

fe::time fe::zero()
    {
        return fe::time();
    }
