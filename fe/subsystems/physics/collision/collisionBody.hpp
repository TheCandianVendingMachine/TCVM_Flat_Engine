// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "collisionData.hpp"
#include "collisionBounds.hpp"
#include "../../../math/Vector2.hpp"
#include <functional>

namespace fe
    {
        static int count = 0;
        struct collider 
            {
                std::function<void(fe::collisionData&)> m_collisionCallback = [](fe::collisionData&) {};

                int id = count++;

                AABB m_aabb;
                void *m_userData; // any additional data that needs to be stored
            };
    }