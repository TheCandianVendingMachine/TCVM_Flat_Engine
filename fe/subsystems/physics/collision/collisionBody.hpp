// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "collisionData.hpp"
#include "../../../math/Vector2.hpp"
#include <functional>

namespace fe
    {
        struct collider 
            {
                float m_positionX;
                float m_positionY;

                std::function<void(fe::collisionData&)> m_collisionCallback = [](fe::collisionData&) {};
            };

        struct AABB : public collider
            {
                float m_sizeX;
                float m_sizeY;
            };
    }