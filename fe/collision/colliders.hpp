// colliders.hpp
// all colliders in the game
#pragma once
#include "../math/Vector2.hpp"

namespace fe
    {
        enum class colliderType
            {
                NONE,
                AABB,
            };

        struct collider
            {
                const Vector2d &m_position;
                colliderType m_type;

                collider(const Vector2d &position) :
                    m_position(position),
                    m_type(colliderType::NONE) {}
            };

        struct AABB : public collider
            {
                AABB(const Vector2d &position) : collider(position) {}
                AABB(const Vector2d &position, const Vector2d &size) :
                    collider(position),
                    m_max(size) { m_type = colliderType::AABB; }

                Vector2d m_min;
                Vector2d m_max;
            };
    }