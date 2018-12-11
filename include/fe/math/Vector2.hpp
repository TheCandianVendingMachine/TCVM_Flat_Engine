// Vector2.hpp
// A 2d vector class used for defining coordinates in space
#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace fe
    {
        template<typename dataType>
        struct lightVector2;

        template <typename dataType>
        struct Vector2
            {
                dataType x;
                dataType y;

                Vector2() : x(0), y(0) {}
                Vector2(dataType X, dataType Y) : x(X), y(Y) {}
                Vector2(const Vector2<dataType> &copy) : x(copy.x), y(copy.y) {}
                Vector2(const lightVector2<dataType> &copy) : x(copy.x), y(copy.y) {}
                Vector2(const sf::Vector2<dataType> &copy) : x(copy.x), y(copy.y) {}

                template <typename otherDataType>
                Vector2(const Vector2<otherDataType> &copy) : x(static_cast<dataType>(copy.x)), y(static_cast<dataType>(copy.y)) {}
                template <typename otherDataType>
                Vector2(const lightVector2<otherDataType> &copy) : x(static_cast<dataType>(copy.x)), y(static_cast<dataType>(copy.y)) {}
                template <typename otherDataType>
                Vector2(const sf::Vector2<otherDataType> &copy) : x(static_cast<dataType>(copy.x)), y(static_cast<dataType>(copy.y)) {}

                Vector2 &operator=(const Vector2<dataType> &copy) { if (&copy != this) { x = copy.x; y = copy.y; } return *this; }
                bool operator==(const Vector2<dataType> &rhs) const { return rhs.x == x && rhs.y == y; }
                bool operator!=(const Vector2<dataType> &rhs) const { return !(rhs == *this); }

                Vector2<dataType> operator+(const Vector2<dataType> &rhs) const { return Vector2<dataType>(rhs.x + x, rhs.y + y); }
                Vector2<dataType> operator-(const Vector2<dataType> &rhs) const { return Vector2<dataType>(x - rhs.x, y - rhs.y); }
                Vector2<dataType> operator*(const dataType &rhs) const          { return Vector2<dataType>(rhs * x, rhs * y); }
                Vector2<dataType> operator/(const dataType &rhs) const          { return Vector2<dataType>(x / rhs, y / rhs); }
                Vector2<dataType> operator-() const                             { return Vector2<dataType>(-x, -y); }
                
                
                template<typename T> Vector2<dataType> operator+(const Vector2<T> &rhs) const   { return Vector2<dataType>(static_cast<dataType>(rhs.x) + x, static_cast<dataType>(rhs.y) + y); }
                template<typename T> Vector2<dataType> operator-(const Vector2<T> &rhs) const   { return Vector2<dataType>(x - static_cast<dataType>(rhs.x), y - static_cast<dataType>(rhs.y)); }
                template<typename T> Vector2<dataType> operator*(const T &rhs) const            { return Vector2<dataType>(static_cast<dataType>(rhs) * x, static_cast<dataType>(rhs) * y); }
                template<typename T> Vector2<dataType> operator/(const T &rhs) const            { return Vector2<dataType>(x / static_cast<dataType>(rhs), y / static_cast<dataType>(rhs)); }

                // A way to get the x/y coordinate based on the index provided. Useful in incrementing loops
                dataType operator[](const size_t &index) const  { if (index == 0) return x; if (index == 1) return y; return 0.f; }
                // A way to get the x/y coordinate based on the index provided. Useful in incrementing loops
                dataType operator[](const int &index) const     { if (index == 0) return x; if (index == 1) return y; return 0.f; }

                void operator+=(const Vector2 &rhs) { x += rhs.x; y += rhs.y; }
                void operator-=(const Vector2 &rhs) { x -= rhs.x; y -= rhs.y; }
                void operator*=(const dataType &rhs){ x *= rhs; y *= rhs; }
                void operator/=(const dataType &rhs){ x /= rhs; y /= rhs; }

                friend fe::Vector2<dataType> operator*(const dataType &lhs, Vector2<dataType> &rhs)
                    {
                        return rhs * lhs;
                    }

                friend fe::Vector2<dataType> operator/(const dataType &lhs, Vector2<dataType> &rhs) 
                    {
                        return rhs * lhs;
                    }

                float magnitude() const { return std::sqrt(x * x + y * y); }
                float magnitudeSqr() const { return x * x + y * y; }

                Vector2<dataType> normalize() const
                    { 
                        float mag = magnitude();
                        if (mag != 0.f)
                            {
                                return Vector2<dataType>(x / mag, y / mag);
                            }
                        return Vector2<dataType>();
                    }

                Vector2<dataType> clamp(float max)
                    {
                        // max^2 / (x^2 + y^2) = 2 * Modifier
                        if (max * max >= x * x + y * y) return *this;
                        float modifier = std::sqrt((max * max) / (x * x + y * y));
                        return modifier < 1.f ? fe::Vector2d(x * modifier, y * modifier) : *this;
                    }

                Vector2<dataType> abs() const { return Vector2(std::abs(x), std::abs(y)); }

                Vector2<dataType> normal() const { return Vector2(-y, x); }

                float dot(const Vector2<dataType> &other) const { return x * other.x + y * other.y; }
                float cross(const Vector2<dataType> &other) const { return x * other.y - y * other.x; }

                Vector2<dataType> project(Vector2<dataType> &other) const
                    {   
                        float mag = other.magnitudeSqr();
                        if (mag != 0) 
                            {
                                return Vector2<dataType>(other * (dot(other) / other.magnitudeSqr()));
                            }

                        return Vector2();
                    }

                sf::Vector2<dataType> convertToSfVec2() { return sf::Vector2<dataType>(x, y); };

            };

        // External functions that are useful for Vector operations
        template<typename dataType>
        Vector2<dataType> lerp(const Vector2<dataType> &a, const Vector2<dataType> &b, const float &percent)
            {
                return Vector2<dataType>(a * (dataType(1) - percent) + (b * percent));
            }

        template<typename T>
        struct lightVector2
            {
                T x;
                T y;

                lightVector2() : x(T()), y(T()) {}
                lightVector2(T x, T y) : x(x), y(y) {}
                lightVector2(const fe::Vector2<T> &copy) : x(copy.x), y(copy.y) {}

                lightVector2<T> operator-(const lightVector2<T> &rhs) { return fe::lightVector2<T>(x - rhs.x, y - rhs.y); }
                lightVector2<T> operator+(const lightVector2<T> &rhs) { return fe::lightVector2<T>(x + rhs.x, y + rhs.y); }
                lightVector2<T> operator-(const fe::Vector2<T> &rhs) { return fe::lightVector2<T>(x - rhs.x, y - rhs.y); }
                lightVector2<T> operator+(const fe::Vector2<T> &rhs) { return fe::lightVector2<T>(x + rhs.x, y + rhs.y); }

                lightVector2<T> operator*(T rhs) { return fe::lightVector2<T>(x * rhs, y * rhs); }
                lightVector2<T> operator/(T rhs) { return fe::lightVector2<T>(x / rhs, y / rhs); }

                sf::Vector2f convertToSfVec2() { return sf::Vector2<T>(x, y); }
            };

        typedef lightVector2<float> lightVector2d;
        typedef Vector2<float> Vector2d;
    }
