#include "collisionManager.hpp"

bool fe::doesCollide(const AABB &first, const AABB &second)
    {
        // Check if any axis doesnt collide another
        if (first.m_max.x + first.m_position.x < second.m_min.x + second.m_position.x || first.m_min.x + first.m_position.x > second.m_max.x + second.m_position.x) return false;
        if (first.m_max.y + first.m_position.y < second.m_min.y + second.m_position.y || first.m_min.y + first.m_position.y > second.m_max.y + second.m_position.y) return false;

        return true;
    }

bool fe::doesContain(const AABB &first, const Vector2d &point)
    {
        return point.x > first.m_min.x + first.m_position.x && point.x < first.m_max.x + first.m_position.x &&
               point.y > first.m_min.y + first.m_position.y && point.y < first.m_max.y + first.m_position.y;
    }

bool fe::doesRayIntersect(AABB &first, Vector2d &origin, Vector2d &direction)
    {
        float tMin = 0.f;
        float tMax = 500000.f; // max distance ray can travel

        const float EPSILON = 0.001f;

        // For all 2 slabs
        for (unsigned int i = 0; i < 2; ++i)
            {
                if (abs(direction[i]) < EPSILON)
                    {
                        // Ray is parallel to slab. Doesnt hit if its not inside
                        if (origin[i] < first.m_min[i] || p[i] > first.m_max[i]) return false;
                    }
            }

        return bool();
    }

bool fe::doesSegmentIntersect(AABB &first, Vector2d &positionA, Vector2d &positionB)
    {
        return bool();
    }
