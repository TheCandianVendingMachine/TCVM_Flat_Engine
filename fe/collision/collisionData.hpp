// collisionData.hpp
// basic data that gets set when a collision happens
#pragma once
#include "../math/Vector2.hpp"

namespace fe
    {
        struct collisionData
            {
                // How deep the collision is
                fe::Vector2d m_collisionDepth;
                int m_collisionGroup;
            };
    }