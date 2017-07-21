// raycastResult.hpp
// A struct that defines what happened during a raycast
#pragma once

namespace fe
    {
        struct collider;
        struct raycastResult
            {
                fe::collider *m_colliderHit;
                // Ray Hit Position
                float m_positionX;
                float m_positionY;
                bool m_hit;
            };
    }