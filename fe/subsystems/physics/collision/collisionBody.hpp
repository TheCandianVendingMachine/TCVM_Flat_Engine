// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "../../../math/Vector2.hpp"
#include <functional>

namespace fe
    {
        struct collisionBody
            {
                union
                    {
                        float m_circleRadius;
                        fe::lightVector2d m_squareSize;
                    } m_colliderSize;

                float m_positionX;
                float m_positionY;
                enum class colliderType
                    {
                        AABB,
                        CIRCLE
                    } m_colliderType;

                std::function<void()> m_collisonCallback;
            };
    }