// transformable.hpp
// a class that will allow objects to be able to be moved when rendered
#pragma once
#define FLAT_ENGINE_API
#include "matrix.hpp"

namespace fe
    {
        class transformable
            {
                private:
                    fe::matrix3d m_matrix;

                    fe::Vector2d m_position;
                    fe::Vector2d m_scale;
                    fe::Vector2d m_origin;
                    float m_rotation;

                    bool m_update;

                public:
                    FLAT_ENGINE_API transformable();

                    // Set the position in the world
                    FLAT_ENGINE_API void setPosition(const fe::Vector2d &position);
                    // Set the rotation in radians
                    FLAT_ENGINE_API void setRotation(float radians);
                    // Set the origin which the object will transform around
                    FLAT_ENGINE_API void setOrigin(const fe::Vector2d &origin);
                    // Set the scale of the object. Overrides all previous scaling
                    FLAT_ENGINE_API void setScale(const fe::Vector2d &scale);
                    

                    // Get the position in the world
                    FLAT_ENGINE_API const fe::Vector2d &getPosition() const;
                    // Get the current scale factor
                    FLAT_ENGINE_API const fe::Vector2d &getScale() const;
                    // Get the position in radians
                    FLAT_ENGINE_API float getRotation() const;

                    // Move the matrix by the offset
                    FLAT_ENGINE_API void move(const fe::Vector2d &offset);
                    // Scale the matrix by the factor
                    FLAT_ENGINE_API void scale(const fe::Vector2d &scale);
                    // Rotate the matrix by an amount of radians
                    FLAT_ENGINE_API void rotate(float radians);

                    FLAT_ENGINE_API const fe::matrix3d &getMatrix();

            };
    }