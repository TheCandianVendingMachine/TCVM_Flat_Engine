// Vector2.hpp
// A 2d vector class used for defining coordinates in space
#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace fe
    {
        template <typename dataType>
        struct Vector2;

        template<typename dataType>
        Vector2<dataType> operator*(const dataType &lhs, Vector2<dataType> &rhs);
        template<typename dataType>
        Vector2<dataType> operator/(const dataType &lhs, Vector2<dataType> &rhs);

        template<typename dataType>
        void operator*=(const dataType &lhs, Vector2<dataType> &rhs);
        template<typename dataType>
        void operator/=(const dataType &lhs, Vector2<dataType> &rhs);

        template <typename dataType>
        struct Vector2
            {
                dataType x;
                dataType y;

                Vector2() : x(0), y(0) {}
                Vector2(dataType X, dataType Y) : x(X), y(Y) {}
                Vector2(const Vector2<dataType> &copy) : x(copy.x), y(copy.y) {}
				Vector2(const sf::Vector2<dataType> &copy) : x(copy.x), y(copy.y) {}

                Vector2 &operator=(const Vector2<dataType> &copy) { if (&copy != this) { x = copy.x; y = copy.y; } return *this; }
                bool operator==(const Vector2<dataType> &rhs) const { return rhs.x == x && rhs.y == y; }

                Vector2<dataType> operator+(const Vector2<dataType> &rhs) const { return Vector2<dataType>(rhs.x + x, rhs.y + y); }
                Vector2<dataType> operator-(const Vector2<dataType> &rhs) const { return Vector2<dataType>(x - rhs.x, y - rhs.y); }
                Vector2<dataType> operator*(const dataType &rhs) const          { return Vector2<dataType>(rhs * x, rhs * y); }
                Vector2<dataType> operator/(const dataType &rhs) const          { return Vector2<dataType>(x / rhs, y / rhs); }
                Vector2<dataType> operator-() const                             { return Vector2<dataType>(-rhs.x, -rhs.y); }
                
                // A way to get the x/y coordinate based on the index provided. Useful in incrementing loops
                dataType operator[](const size_t &index) const  { if (index == 0) return x; if (index == 1) return y; return 0.f; }
                // A way to get the x/y coordinate based on the index provided. Useful in incrementing loops
                dataType operator[](const int &index) const     { if (index == 0) return x; if (index == 1) return y; return 0.f; }

                void operator+=(const Vector2 &rhs) { x += rhs.x; y += rhs.y; }
                void operator-=(const Vector2 &rhs) { x -= rhs.x; y -= rhs.y; }
                void operator*=(const dataType &rhs){ x *= rhs; y *= rhs; }
                void operator/=(const dataType &rhs){ x /= rhs; y /= rhs; }

                friend Vector2<dataType> operator*(const dataType &lhs, Vector2<dataType> &rhs);
                friend Vector2<dataType> operator/(const dataType &lhs, Vector2<dataType> &rhs);

                friend void operator*=(const dataType &lhs, Vector2<dataType> &rhs);
                friend void operator/=(const dataType &lhs, Vector2<dataType> &rhs);

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
                        float modifier = std::sqrt((max * max) / (x * x + y * y));
                        return modifier < 1.f ? fe::Vector2d(x * modifier, y * modifier) : *this;
                    }

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
                return Vector2<dataType>((1 - percent) * a + (percent * b));
            }

        template<typename dataType>
        Vector2<dataType> fe::operator*(const dataType &lhs, Vector2<dataType> &rhs)
            {
                return rhs * lhs;
            }

        template<typename dataType>
        Vector2<dataType> fe::operator/(const dataType &lhs, Vector2<dataType> &rhs)
            {
                return rhs / lhs;
            }

        template<typename dataType>
        void fe::operator*=(const dataType &lhs, Vector2<dataType> &rhs)
            {
                rhs *= lhs;
            }

        template<typename dataType>
        void fe::operator/=(const dataType &lhs, Vector2<dataType> &rhs)
            {
                rhs /= lhs;
            }


        typedef Vector2<float> Vector2d;
    }