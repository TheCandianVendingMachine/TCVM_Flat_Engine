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
                    {

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
            };
    }