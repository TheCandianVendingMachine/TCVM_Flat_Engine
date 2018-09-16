// raycastResult.hpp
// A struct that defines what happened during a raycast
#pragma once

namespace fe
    {
        struct collider;
        struct raycastResult
            {
                fe::collider *m_colliderHit = nullptr;
                // Ray Hit Position
                float m_positionX = 0.f;
                float m_positionY = 0.f;
                bool m_hit = false;
            };
    }