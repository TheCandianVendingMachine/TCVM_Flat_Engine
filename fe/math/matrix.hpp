// matrix.hpp
// A 2d matrix to allow for easy transformations
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include <utility>

namespace fe 
    {
        struct matrix3d
            {
                typedef float matrix[9];

                matrix values;

                matrix3d()
                    {}

                matrix3d(float x1, float y1, float z1,
                         float x2, float y2, float z2,
                         float x3, float y3, float z3)
                    {
                        values[0] = x1;
                        values[1] = y1;
                        values[2] = z1;

                        values[3] = x2;
                        values[4] = y2;
                        values[5] = z2;

                        values[6] = x3;
                        values[7] = y3;
                        values[8] = z3;
                    }

                matrix3d &operator=(const matrix3d &rhs)
                    {
                        if (this != &rhs) 
                            {
                                std::memcpy(values, rhs.values, sizeof(rhs.values));
                            }

                        return *this;
                    }

                void operator+=(const matrix3d &other)
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
                void operator-=(const matrix3d &other)
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

                void operator*=(const float &rhs)
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
                void operator/=(const float &rhs)
                    {
                        values[0] /= rhs;
                        values[1] /= rhs;
                        values[2] /= rhs;
                        values[3] /= rhs;
                        values[4] /= rhs;
                        values[5] /= rhs;
                        values[6] /= rhs;
                        values[7] /= rhs;
                        values[8] /= rhs;
                    }

                void operator*=(const matrix3d &rhs)
                    {
                        const matrix &a = values;
                        const matrix &b = rhs.values;

                        matrix result = 
                            {
                               a[0]*b[0] + a[1]*b[3] + a[2]*b[6], a[0]*b[1] + a[1]*b[4] + a[3]*b[7], a[0]*b[2] + a[1]*b[5] + a[3]*b[8],
                               a[3]*b[0] + a[4]*b[3] + a[5]*b[6], a[3]*b[1] + a[4]*b[4] + a[5]*b[7], a[3]*b[2] + a[4]*b[5] + a[5]*b[8],
                               a[6]*b[0] + a[7]*b[3] + a[8]*b[6], a[6]*b[1] + a[7]*b[4] + a[8]*b[7], a[6]*b[2] + a[7]*b[5] + a[8]*b[8]
                            };

                        std::memcpy(values, result, sizeof(result));
                    }
 
            };
    }