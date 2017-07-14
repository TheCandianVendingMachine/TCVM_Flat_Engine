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
            };
    }