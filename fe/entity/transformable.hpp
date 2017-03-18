// transformable.hpp
// a class that will allow objects to be able to be moved when rendered
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../math/matrix.hpp"

namespace fe
    {
        class transformable
            {
                private:
                    fe::matrix3d m_matrix;

                    fe::Vector2d m_position;
                    float m_rotation;

                    bool m_update;

                public:
                    FLAT_ENGINE_API transformable();

                    // Set the position in the world
                    FLAT_ENGINE_API void setPosition(const fe::Vector2d &position);
                    // Set the rotation in radians
                    FLAT_ENGINE_API void setRotation(const float &radians);
                    // Set the origin which the object will transform around
                    FLAT_ENGINE_API void setOrigin(const fe::Vector2d &origin);

                    // Get the position in the world
                    FLAT_ENGINE_API fe::Vector2d getPosition();
                    // Get the position in radians
                    FLAT_ENGINE_API float getRotation();

                    // Move the matrix by the offset
                    FLAT_ENGINE_API void move(const fe::Vector2d &offset);
                    // Rotate the matrix by an amount of radians
                    FLAT_ENGINE_API void rotate(const float &radians);

                    FLAT_ENGINE_API const fe::matrix3d &getMatrix();

            };
    }