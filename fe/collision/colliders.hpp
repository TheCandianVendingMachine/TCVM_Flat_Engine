// colliders.hpp
// all colliders in the game
#pragma once
#include "../math/Vector2.hpp"

namespace fe
    {
        struct collider
            {
                const Vector2d &m_position;

                collider(const Vector2d &position) : m_position(position) {}
            };

        struct AABB : public collider
            {
                AABB(const Vector2d &position) : collider(position) {}
                AABB(const Vector2d &position, const Vector2d &size) : collider(position), m_max(size) {}

                Vector2d m_min;
                Vector2d m_max;
            };
    }