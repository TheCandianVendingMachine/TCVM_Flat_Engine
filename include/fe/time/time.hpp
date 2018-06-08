// time.hpp
// A class to store time values
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include <cstdint>

namespace fe 
    {
        class time
            {
                private:
                    // time as microseconds
                    int64_t m_time;

                public:
                    FLAT_ENGINE_API time();
                    FLAT_ENGINE_API time(long long time);
                    FLAT_ENGINE_API time(const fe::time &time);
                    FLAT_ENGINE_API time(const fe::time &&time);

                    FLAT_ENGINE_API double asSeconds();
                    FLAT_ENGINE_API int32_t asMilliseconds();
                    FLAT_ENGINE_API int64_t asMicroseconds();

                    FLAT_ENGINE_API fe::time operator+(const fe::time &rhs) const;
                    FLAT_ENGINE_API fe::time operator-(const fe::time & rhs) const;
                    FLAT_ENGINE_API void operator+=(const fe::time &rhs);
                    FLAT_ENGINE_API void operator-=(const fe::time &rhs);

                    FLAT_ENGINE_API fe::time &operator=(const fe::time &rhs);
                    FLAT_ENGINE_API fe::time operator/(unsigned int rhs);
                    FLAT_ENGINE_API void operator/=(unsigned int rhs);

                    FLAT_ENGINE_API bool operator>(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator>=(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator<(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator<=(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator==(const fe::time &rhs);

            };

        FLAT_ENGINE_API fe::time seconds(const long long in);
        FLAT_ENGINE_API fe::time milliseconds(const long long in);
        FLAT_ENGINE_API fe::time microseconds(const long long in);
        FLAT_ENGINE_API fe::time zero();
    }