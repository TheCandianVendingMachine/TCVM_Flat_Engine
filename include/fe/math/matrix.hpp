// matrix.hpp
// A 2d matrix to allow for easy transformations
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "Vector2.hpp"
#include <cstring>
#include <cmath>

// Converts the wanted degrees to radians
constexpr float operator "" _Deg(long double degrees)
    {
        return static_cast<float>(static_cast<long double>(degrees) * 3.14159f / 180.f);
    }

// Converts the wanted degrees to radians
constexpr float operator "" _Deg(unsigned long long degrees)
    {
        return static_cast<float>(static_cast<long double>(degrees) * 3.14159f / 180.f);
    }

namespace fe 
    {
        struct matrix3d;
        FLAT_ENGINE_API matrix3d operator*(float lhs, const matrix3d &rhs);
        FLAT_ENGINE_API matrix3d operator+(float lhs, const matrix3d &rhs);
        FLAT_ENGINE_API matrix3d operator-(float lhs, const matrix3d &rhs);

        struct matrix3d
            {
                typedef float matrix[9];
                matrix values;

                FLAT_ENGINE_API matrix3d();
                FLAT_ENGINE_API matrix3d(float x11, float y12, float z13,
                                         float x21, float y22, float z23,
                                         float x31, float y32, float z33);

                FLAT_ENGINE_API matrix3d &operator=(const matrix3d &rhs);
                FLAT_ENGINE_API matrix3d operator*(const float &rhs) const;
                FLAT_ENGINE_API matrix3d operator+(const float &rhs) const;
                FLAT_ENGINE_API matrix3d operator-(const float &rhs) const;

                FLAT_ENGINE_API void operator+=(const matrix3d &other);
                FLAT_ENGINE_API void operator-=(const matrix3d &other);
                FLAT_ENGINE_API void operator*=(const float &rhs);

                FLAT_ENGINE_API void operator*=(const matrix3d &rhs);
                FLAT_ENGINE_API matrix3d operator*(const matrix3d &rhs) const;

                FLAT_ENGINE_API float operator[](unsigned int index) const;

                FLAT_ENGINE_API void combine(const matrix3d &rhs);

                FLAT_ENGINE_API matrix3d transpose();

                FLAT_ENGINE_API void translate(fe::lightVector2d &&translation);
                FLAT_ENGINE_API void rotate(float radians);

                FLAT_ENGINE_API fe::lightVector2d translatePoint(const fe::lightVector2d &&point) const;
                FLAT_ENGINE_API fe::lightVector2d rotatePoint(const fe::lightVector2d &&point) const;
                FLAT_ENGINE_API fe::lightVector2d rotatePoint(const fe::lightVector2d &&point, float radians) const;

                // combines the rotation, scale, and translation of the matrix and applies it to the point
                FLAT_ENGINE_API fe::lightVector2d transformPoint(const fe::lightVector2d &&point) const;

                FLAT_ENGINE_API fe::lightVector2d translatePointToLocalSpace(const fe::lightVector2d &&point) const;
                FLAT_ENGINE_API fe::lightVector2d rotatePointToLocalSpace(const fe::lightVector2d &&point) const;
                FLAT_ENGINE_API fe::lightVector2d rotatePointToLocalSpace(const fe::lightVector2d &&point, float radians) const;

                // transforms the point such that (0, 0) is the matricies position
                FLAT_ENGINE_API fe::lightVector2d transformPointToLocalSpace(const fe::lightVector2d &&point) const;

            };
    }