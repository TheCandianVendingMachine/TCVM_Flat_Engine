// collisionData.hpp
// Data for when an object collides with another
#pragma once

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

                // The normalized normal of the surface collided with
                float m_normalX;
                float m_normalY;

            };
    }