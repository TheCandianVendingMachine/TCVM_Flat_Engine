// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "collisionData.hpp"
#include "../../../math/Vector2.hpp"
#include <functional>

namespace fe
    {
        struct AABB
            {
                float m_positionX;
                float m_positionY;

                float m_sizeX;
                float m_sizeY;

                std::function<void(fe::collisionData&)> m_collisionCallback = [](fe::collisionData&) {};
            };
    }