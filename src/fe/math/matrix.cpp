#include "fe/math/matrix.hpp"
#include <utility>

fe::matrix3d::matrix3d()
    {
        // creates an identiy matrix
        values[0] = 1.f;
        values[4] = 1.f;
        values[8] = 1.f;

        values[1] = 0.f;
        values[2] = 0.f;
        values[3] = 0.f;
        values[5] = 0.f;
        values[6] = 0.f;
        values[7] = 0.f;
    }

fe::matrix3d::matrix3d(float x11, float y12, float z13, float x21, float y22, float z23, float x31, float y32, float z33)
    {
        values[0] = x11;
        values[1] = y12;
        values[2] = z13;

        values[3] = x21;
        values[4] = y22;
        values[5] = z23;

        values[6] = x31;
        values[7] = y32;
        values[8] = z33;
    }

fe::matrix3d &fe::matrix3d::operator=(const matrix3d &rhs)
    {
        if (this != &rhs) 
            {
                std::memcpy(values, rhs.values, sizeof(rhs.values));
            }

        return *this;
    }

fe::matrix3d fe::matrix3d::operator*(const float &rhs) const
    {
        return matrix3d(values[0] * rhs, values[1] * rhs, values[2] * rhs,
                        values[3] * rhs, values[4] * rhs, values[5] * rhs,
                        values[6] * rhs, values[7] * rhs, values[8] * rhs);
    }

fe::matrix3d fe::matrix3d::operator+(const float &rhs) const
    {
        return matrix3d(values[0] + rhs, values[1] + rhs, values[2] + rhs,
                        values[3] + rhs, values[4] + rhs, values[5] + rhs,
                        values[6] + rhs, values[7] + rhs, values[8] + rhs);
    }

fe::matrix3d fe::matrix3d::operator-(const float & rhs) const
    {
        return matrix3d(values[0] - rhs, values[1] - rhs, values[2] - rhs,
                        values[3] - rhs, values[4] - rhs, values[5] - rhs,
                        values[6] - rhs, values[7] - rhs, values[8] - rhs);
    }

void fe::matrix3d::operator+=(const matrix3d &other)
    {
        values[0] += other.values[0];
        values[1] += other.values[1];
        values[2] += other.values[2];
        values[3] += other.values[3];
        values[4] += other.values[4];
        values[5] += other.values[5];
        values[6] += other.values[6];
        values[7] += other.values[7];
        values[8] += other.values[8];
    }

void fe::matrix3d::operator-=(const matrix3d &other)
    {
        values[0] -= other.values[0];
        values[1] -= other.values[1];
        values[2] -= other.values[2];
        values[3] -= other.values[3];
        values[4] -= other.values[4];
        values[5] -= other.values[5];
        values[6] -= other.values[6];
        values[7] -= other.values[7];
        values[8] -= other.values[8];
    }

void fe::matrix3d::operator*=(const float &rhs)
    {
        values[0] *= rhs;
        values[1] *= rhs;
        values[2] *= rhs;
        values[3] *= rhs;
        values[4] *= rhs;
        values[5] *= rhs;
        values[6] *= rhs;
        values[7] *= rhs;
        values[8] *= rhs;
    }

void fe::matrix3d::operator*=(const matrix3d &rhs)
    {
        combine(rhs);
    }

fe::matrix3d fe::matrix3d::operator*(const matrix3d &rhs) const
    {
        matrix3d ret;
        ret.combine(rhs);
        return ret;
    }

float fe::matrix3d::operator[](unsigned int index) const
    {
        return values[index];
    }

void fe::matrix3d::combine(const fe::matrix3d &rhs)
    {
        float x = values[0] * rhs.values[0] + values[1] * rhs.values[3] + values[2] * rhs.values[6]; // 11
        float y = values[0] * rhs.values[1] + values[1] * rhs.values[4] + values[2] * rhs.values[7]; // 21
        float z = values[0] * rhs.values[2] + values[1] * rhs.values[5] + values[2] * rhs.values[8]; // 31

        values[0] = x;
        values[1] = y;
        values[2] = z;

        x = values[3] * rhs.values[0] + values[4] * rhs.values[3] + values[5] * rhs.values[6]; // 12
        y = values[3] * rhs.values[1] + values[4] * rhs.values[4] + values[5] * rhs.values[7]; // 22
        z = values[3] * rhs.values[2] + values[4] * rhs.values[5] + values[5] * rhs.values[8]; // 32

        values[3] = x;
        values[4] = y;
        values[5] = z;

        x = values[6] * rhs.values[0] + values[7] * rhs.values[3] + values[8] * rhs.values[6]; // 13
        y = values[6] * rhs.values[1] + values[7] * rhs.values[4] + values[8] * rhs.values[7]; // 23
        z = values[6] * rhs.values[2] + values[7] * rhs.values[5] + values[8] * rhs.values[8]; // 33

        values[6] = x; // Position X
        values[7] = y; // Position Y
        values[8] = z;
    }

fe::matrix3d fe::matrix3d::transpose()
    {
        return matrix3d(values[0], values[3], values[6],
                        values[1], values[4], values[7],
                        values[2], values[5], values[8]);
    }

void fe::matrix3d::translate(fe::lightVector2d &&translation)
    {
        values[6] += translation.x;
        values[7] += translation.y;
    }

void fe::matrix3d::rotate(float radians)
    {
        *this *= fe::matrix3d(cos(radians), -sin(radians), 0,
                                sin(radians), cos(radians),  0,
                                0,            0,             1);
    }

fe::lightVector2d fe::matrix3d::translatePoint(const fe::lightVector2d &&point) const
    {
        return fe::lightVector2d(values[6] + point.x, values[7] + point.y);
    }

fe::lightVector2d fe::matrix3d::rotatePoint(const fe::lightVector2d &&point) const
    {
        return fe::lightVector2d(point.x * values[0] + point.y * values[1],
                                 point.x * values[3] + point.y * values[4]);
    }

fe::lightVector2d fe::matrix3d::rotatePoint(const fe::lightVector2d &&point, float radians) const
    {
        return (*this * fe::matrix3d(cos(radians), -sin(radians), 0,
                                     sin(radians), cos(radians),  0,
                                     0,            0,             1)).rotatePoint(std::forward<const fe::lightVector2d>(point));
    }

fe::lightVector2d fe::matrix3d::transformPoint(const fe::lightVector2d &&point) const
    {
        fe::lightVector2d transformed = point;
        transformed = rotatePoint(std::forward<const fe::lightVector2d>(transformed));
        transformed = translatePoint(std::forward<const fe::lightVector2d>(transformed));
        
        return transformed;
    }

fe::lightVector2d fe::matrix3d::translatePointToLocalSpace(const fe::lightVector2d &&point) const
    {
        return fe::lightVector2d(point.x - values[6], point.y - values[7]);
    }

fe::lightVector2d fe::matrix3d::rotatePointToLocalSpace(const fe::lightVector2d &&point) const
    {
        return fe::lightVector2d(point.x * values[0] + point.y * -values[1],
                                 point.x * -values[3] + point.y * values[4]);
    }

fe::lightVector2d fe::matrix3d::rotatePointToLocalSpace(const fe::lightVector2d &&point, float radians) const
    {
        return (*this * fe::matrix3d(cos(radians), -sin(radians), 0,
                                     sin(radians), cos(radians),  0,
                                     0,            0,             1)).rotatePointToLocalSpace(std::forward<const fe::lightVector2d>(point));
    }

fe::lightVector2d fe::matrix3d::transformPointToLocalSpace(const fe::lightVector2d &&point) const
    {
        fe::lightVector2d transformed = point;
        transformed = translatePointToLocalSpace(std::forward<const fe::lightVector2d>(transformed));
        transformed = rotatePointToLocalSpace(std::forward<const fe::lightVector2d>(transformed));

        return transformed;
    }

fe::matrix3d operator*(float lhs, const fe::matrix3d &rhs)
    {
        return fe::matrix3d(rhs.values[0] * lhs, rhs.values[1] * lhs, rhs.values[2] * lhs,
                            rhs.values[3] * lhs, rhs.values[4] * lhs, rhs.values[5] * lhs,
                            rhs.values[6] * lhs, rhs.values[7] * lhs, rhs.values[8] * lhs);
    }

fe::matrix3d operator+(float lhs, const fe::matrix3d &rhs)
    {
        return fe::matrix3d(rhs.values[0] + lhs, rhs.values[1] + lhs, rhs.values[2] + lhs,
                            rhs.values[3] + lhs, rhs.values[4] + lhs, rhs.values[5] + lhs,
                            rhs.values[6] + lhs, rhs.values[7] + lhs, rhs.values[8] + lhs);
    }

fe::matrix3d operator-(float lhs, const fe::matrix3d &rhs)
    {
        return fe::matrix3d(rhs.values[0] - lhs, rhs.values[1] - lhs, rhs.values[2] - lhs,
                            rhs.values[3] - lhs, rhs.values[4] - lhs, rhs.values[5] - lhs,
                            rhs.values[6] - lhs, rhs.values[7] - lhs, rhs.values[8] - lhs);
    }