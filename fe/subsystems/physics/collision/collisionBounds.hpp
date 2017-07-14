// collisionBounds.hpp
// A group of structs that represent different types of bounds
#pragma once

namespace fe
    {
        struct collisionBounds
            {
                float m_positionX;
                float m_positionY;
            };

        struct AABB : collisionBounds
            {
                float m_sizeX;
                float m_sizeY;

                bool intersects(const fe::AABB *other) const;
                bool contains(const fe::AABB *other) const;
                fe::AABB merge(const fe::AABB *other);
                float volume() const;
            };
    }