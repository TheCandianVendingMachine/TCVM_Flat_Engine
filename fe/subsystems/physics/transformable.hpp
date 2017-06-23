// transformable.hpp
// a class that will allow objects to be able to be moved when rendered
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../math/matrix.hpp"

namespace fe
    {
        class transformable
            {
                private:
                    fe::matrix3d m_matrix;

                    float m_positionX;
                    float m_positionY;

                    float m_scaleX;
                    float m_scaleY;

                    float m_originX;
                    float m_originY;

                    float m_rotation;

                    bool m_update;

                public:
                    FLAT_ENGINE_API transformable();

                    // Set the position in the world
                    FLAT_ENGINE_API void setPosition(float x, float y);
                    // Set the rotation in radians
                    FLAT_ENGINE_API void setRotation(float radians);
                    // Set the origin which the object will transform around
                    FLAT_ENGINE_API void setOrigin(float x, float y);
                    // Set the scale of the object. Overrides all previous scaling
                    FLAT_ENGINE_API void setScale(float x, float y);
                    // Set the scale of the object. Overrides all previous scaling
                    FLAT_ENGINE_API void setScale(float scale);
                    

                    // Get the position in the world
                    FLAT_ENGINE_API fe::Vector2d getPosition() const;
                    // Get the current scale factor
                    FLAT_ENGINE_API fe::Vector2d getScale() const;
                    // Get the position in radians
                    FLAT_ENGINE_API float getRotation() const;
                    // Get the size of an object if transformed via this matrix
                    FLAT_ENGINE_API fe::Vector2d getSize(float x, float y) const;

                    // Move the matrix by the offset
                    FLAT_ENGINE_API void move(float x, float y);
                    // Scale the matrix by the factor
                    FLAT_ENGINE_API void scale(float x, float y);
                    // Scale the matrix by the factor
                    FLAT_ENGINE_API void scale(float scale);
                    // Rotate the matrix by an amount of radians
                    FLAT_ENGINE_API void rotate(float radians);

                    FLAT_ENGINE_API const fe::matrix3d &getMatrix();

            };
    }