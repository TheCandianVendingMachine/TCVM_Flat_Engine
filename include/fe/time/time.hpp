// time.hpp
// A class to store time values
#pragma once
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

                    template<typename T>
                    fe::time operator*(T rhs);
                    template<typename T>
                    fe::time operator/(T rhs);

                    template<typename T>
                    void operator*=(T rhs);
                    template<typename T>
                    void operator/=(T rhs);

                    FLAT_ENGINE_API bool operator>(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator>=(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator<(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator<=(const fe::time &rhs);
                    FLAT_ENGINE_API bool operator==(const fe::time &rhs);

            };

        template<typename T>
        fe::time seconds(T in);

        template<typename T>
        fe::time milliseconds(T in);

        template<typename T>
        fe::time microseconds(T in);

        FLAT_ENGINE_API fe::time zero();

        template<typename T>
        fe::time seconds(T in)
            {
                return fe::time(in * 1000000);
            }

        template<typename T>
        fe::time milliseconds(T in)
            {
                return fe::time(in * 1000);
            }

        template<typename T>
        fe::time microseconds(T in)
            {
                return fe::time(in);
            }

        template<typename T>
        inline fe::time time::operator*(T rhs)
            {
                return fe::time(m_time * rhs);
            }

        template<typename T>
        inline fe::time time::operator/(T rhs)
            {
                return fe::time(m_time / rhs);
            }

        template<typename T>
        void fe::time::operator*=(T rhs)
            {
                m_time *= rhs;
            }

        template<typename T>
        void fe::time::operator/=(T rhs)
            {
                m_time /= rhs;
            }
    }