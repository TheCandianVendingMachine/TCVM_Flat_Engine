// raycastResult.hpp
// A struct that defines what happened during a raycast
#pragma once

namespace fe
    {
        class collider;
        struct raycastResult
            {
                fe::collider *m_colliderHit;
                float m_positionX;
                float m_positionY;
                bool m_hit;
            };
    }