// time.hpp
// A class to store time values
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineInclude.hpp"
#include <cstdint>

namespace fe 
    {
        class FLAT_ENGINE_API time
            {
                private:
                    // time as microseconds
                    long long m_time;

                    friend FLAT_ENGINE_API fe::time seconds(const long long in);
                    friend FLAT_ENGINE_API fe::time milliseconds(const long long in);
                    friend FLAT_ENGINE_API fe::time microseconds(const long long in);

                public:
                    time();

                    float asSeconds();
                    int32_t asMilliseconds();
                    int64_t asMicroseconds();

                    fe::time operator+(const fe::time &rhs);
                    fe::time operator-(const fe::time & rhs);
                    void operator+=(const fe::time &rhs);
                    void operator-=(const fe::time &rhs);

                    bool operator==(const fe::time &rhs);

            };

        FLAT_ENGINE_API fe::time seconds(const long long in);
        FLAT_ENGINE_API fe::time milliseconds(const long long in);
        FLAT_ENGINE_API fe::time microseconds(const long long in);
        FLAT_ENGINE_API fe::time zero();
    }