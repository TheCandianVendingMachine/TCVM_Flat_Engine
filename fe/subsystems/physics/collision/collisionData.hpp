// collisionData.hpp
// Data for when an object collides with another
#pragma once
#include "collisionBody.hpp"
#include "colliderTypes.hpp"

namespace fe
    {
        struct collisionData
            {
                // The position of the other collider
                float m_colliderPositionX;
                float m_colliderPositionY;

                // How deep the object penetrated into the other
                float m_penetrationX;
                float m_penetrationY;

                // The type of collider the other body was
                fe::colliderType m_colliderType;
            };
    }