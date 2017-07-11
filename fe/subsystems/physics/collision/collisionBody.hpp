// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "colliderTypes.hpp"
#include "../../../math/Vector2.hpp"
#include <functional>

namespace fe
    {
        struct collisionData;
        struct collisionBody
            {
                union colliderSize
                    {
                        float m_circleRadius;
                        fe::lightVector2d m_squareSize;

                        colliderSize() { std::memset(this, 0, sizeof(colliderSize)); }
                    } m_colliderSize;

                float m_positionX;
                float m_positionY;
                fe::colliderType m_colliderType;

                std::function<void(fe::collisionData&)> m_collisonCallback;
            };
    }