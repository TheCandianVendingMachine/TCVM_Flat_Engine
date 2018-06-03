// Vector3.hpp
// A 3d vector class used for defining coordinates in space
#pragma once
#include <cmath>
#include <SFML/System/Vector3.hpp>

namespace fe
    {
        template <typename dataType>
        struct Vector3;
        template <typename T>
        struct lightVector3;

        template<typename dataType, typename vectorType>
        Vector3<dataType> operator*(const dataType &lhs, Vector3<vectorType> &rhs);
        template<typename dataType, typename vectorType>
        Vector3<dataType> operator/(const dataType &lhs, Vector3<vectorType> &rhs);

        template<typename dataType, typename vectorType>
        void operator*=(const dataType &lhs, Vector3<vectorType> &rhs);
        template<typename dataType, typename vectorType>
        void operator/=(const dataType &lhs, Vector3<vectorType> &rhs);

        template <typename dataType>
        struct Vector3
            {
                dataType x;
                dataType y;
                dataType z;

                Vector3() : x(0), y(0), z(0) {}
                Vector3(dataType X, dataType Y, dataType Z) : x(X), y(Y), z(Z) {}
                Vector3(const Vector3<dataType> &copy) : x(copy.x), y(copy.y), z(copy.z) {}
                Vector3(const lightVector3<dataType> &copy) : x(copy.x), y(copy.y), z(copy.z) {}
				Vector3(const sf::Vector3<dataType> &copy) : x(copy.x), y(copy.y), z(copy.z) {}

                template <typename otherDataType>
                Vector3(const Vector3<otherDataType> &copy) : x(static_cast<dataType>(copy.x)), y(static_cast<dataType>(copy.y)), z(static_cast<dataType>(copy.z)) {}
                template <typename otherDataType>
                Vector3(const lightVector3<otherDataType> &copy) : x(static_cast<dataType>(copy.x)), y(static_cast<dataType>(copy.y)), z(static_cast<dataType>(copy.z)) {}
                template <typename otherDataType>
                Vector3(const sf::Vector3<otherDataType> &copy) : x(static_cast<dataType>(copy.x)), y(static_cast<dataType>(copy.y)), z(static_cast<dataType>(copy.z)) {}

                Vector3 &operator=(const Vector3<dataType> &copy) { if (&copy != this) { x = copy.x; y = copy.y; z = copy.z; } return *this; }
                bool operator==(const Vector3<dataType> &rhs) const { return rhs.x == x && rhs.y == y && rhs.y == y; }

                Vector3<dataType> operator+(const Vector3<dataType> &rhs) const { return Vector3<dataType>(rhs.x + x, rhs.y + y, rhs.z + z); }
                Vector3<dataType> operator-(const Vector3<dataType> &rhs) const { return Vector3<dataType>(x - rhs.x, y - rhs.y, z - rhs.z); }
                Vector3<dataType> operator*(const dataType &rhs) const          { return Vector3<dataType>(rhs * x, rhs * y, rhs * z); }
                Vector3<dataType> operator/(const dataType &rhs) const          { return Vector3<dataType>(x / rhs, y / rhs, z / rhs); }
                Vector3<dataType> operator-() const                             { return Vector3<dataType>(-x, -y, -z); }
                
                
                template<typename T> Vector3<dataType> operator+(const Vector3<T> &rhs) const   { return Vector3<dataType>(static_cast<dataType>(rhs.x) + x, static_cast<dataType>(rhs.y) + y, static_cast<dataType>(rhs.z) + z); }
                template<typename T> Vector3<dataType> operator-(const Vector3<T> &rhs) const   { return Vector3<dataType>(x - static_cast<dataType>(rhs.x), y - static_cast<dataType>(rhs.y), z - static_cast<dataType>(rhs.z)); }
                template<typename T> Vector3<dataType> operator*(const T &rhs) const            { return Vector3<dataType>(static_cast<dataType>(rhs) * x, static_cast<dataType>(rhs) * y, static_cast<dataType>(rhs) * z); }
                template<typename T> Vector3<dataType> operator/(const T &rhs) const            { return Vector3<dataType>(x / static_cast<dataType>(rhs), y / static_cast<dataType>(rhs), z / static_cast<dataType>(rhs)); }

                // A way to get the x/y coordinate based on the index provided. Useful in incrementing loops
                dataType operator[](const size_t &index) const  { if (index == 0) return x; if (index == 1) return y; if (index == 2) return z; return 0.f; }
                // A way to get the x/y coordinate based on the index provided. Useful in incrementing loops
                dataType operator[](const int &index) const     { if (index == 0) return x; if (index == 1) return y; if (index == 2) return z; return 0.f; }

                void operator+=(const Vector3 &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; }
                void operator-=(const Vector3 &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
                void operator*=(const dataType &rhs){ x *= rhs; y *= rhs; z *= rhs; }
                void operator/=(const dataType &rhs){ x /= rhs; y /= rhs; z /= rhs; }

                friend Vector3<dataType> operator*(const dataType &lhs, Vector3<dataType> &rhs);
                friend Vector3<dataType> operator/(const dataType &lhs, Vector3<dataType> &rhs);

                friend void operator*=(const dataType &lhs, Vector3<dataType> &rhs);
                friend void operator/=(const dataType &lhs, Vector3<dataType> &rhs);

                float magnitude() const { return std::sqrt(x * x + y * y + z * z); }
                float magnitudeSqr() const { return x * x + y * y + z * z; }

                Vector3<dataType> normalize() const
                    { 
                        float mag = magnitude();
                        if (mag != 0.f)
                            {
                                return Vector3<dataType>(x / mag, y / mag, z / mag);
                            }
                        return Vector3<dataType>();
                    }

                Vector3<dataType> clamp(float max)
                    {
                        // max^3 / (x^3 + y^3) = 3 * Modifier
                        if (max * max > x * x + y * y + z * z) return *this;
                        float modifier = std::sqrt((max * max) / (x * x + y * y + z * z));
                        return modifier < 1.f ? fe::Vector3d(x * modifier, y * modifier, z * modifier) : *this;
                    }

                Vector3<dataType> abs() const { return Vector3(std::abs(x), std::abs(y), std::abs(z)); }

                Vector3<dataType> normal() const { return Vector3(-y, x, z); }

                float dot(const Vector3<dataType> &other) const { return x * other.x + y * other.y + z * other.z; }
                Vector3<dataType> cross(const Vector3<dataType> &other) const { return Vector3<dataType>(y * other.y - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }

                Vector3<dataType> project(Vector3<dataType> &other) const
                    {   
                        float mag = other.magnitudeSqr();
                        if (mag != 0) 
                            {
                                return Vector3<dataType>(other * (dot(other) / other.magnitudeSqr()));
                            }

                        return Vector3();
                    }
            };

        // External functions that are useful for Vector operations
        template<typename dataType>
        Vector3<dataType> lerp(const Vector3<dataType> &a, const Vector3<dataType> &b, const float &percent)
            {
                return Vector3<dataType>((dataType(1) - percent) * a + (percent * b));
            }

        template<typename dataType, typename vectorType>
        Vector3<dataType> fe::operator*(const dataType &lhs, Vector3<vectorType> &rhs)
            {
                return rhs * lhs;
            }

        template<typename dataType, typename vectorType>
        Vector3<dataType> fe::operator/(const dataType &lhs, Vector3<vectorType> &rhs)
            {
                return rhs / lhs;
            }

        template<typename dataType, typename vectorType>
        void fe::operator*=(const dataType &lhs, Vector3<vectorType> &rhs)
            {
                rhs *= lhs;
            }

        template<typename dataType, typename vectorType>
        void fe::operator/=(const dataType &lhs, Vector3<vectorType> &rhs)
            {
                rhs /= lhs;
            }

        template<typename T>
        struct lightVector3
            {
                T x;
                T y;
                T z;

                lightVector3() : x(T()), y(T()), z(T()) {}
                lightVector3(T x, T y, T z) : x(x), y(y), z(z) {}
                lightVector3(const fe::Vector3<T> &copy) : x(copy.x), y(copy.y), z(copy.z) {}

                lightVector3<T> operator-(const lightVector3<T> &rhs) { return fe::lightVector3<T>(x - rhs.x, y - rhs.y, z - rhs.z); }
                lightVector3<T> operator+(const lightVector3<T> &rhs) { return fe::lightVector3<T>(x + rhs.x, y + rhs.y, z + rhs.z); }
                lightVector3<T> operator-(const fe::Vector3<T> &rhs) { return fe::lightVector3<T>(x - rhs.x, y - rhs.y, z - rhs.z); }
                lightVector3<T> operator+(const fe::Vector3<T> &rhs) { return fe::lightVector3<T>(x + rhs.x, y + rhs.y, z + rhs.z); }

                lightVector2<T> operator*(T rhs) { return fe::lightVector2<T>(x * rhs, y * rhs, z * rhs); }
                lightVector2<T> operator/(T rhs) { return fe::lightVector2<T>(x / rhs, y / rhs, z / rhs); }

            };

        typedef lightVector3<float> lightVector3d;
        typedef Vector3<float> Vector3d;
    }
