#include "colliders.hpp"
#include <algorithm>

fe::AABB::AABB(const Vector2d size) : m_max(size)
    {
        m_type = colliderType::AABB;
    }

fe::AABB::AABB(const Vector2d size, std::function<void(const collider&)> callback) : m_max(size)
    {
        m_type = colliderType::AABB;
        m_callback = callback;
    }

bool fe::AABB::collide(const collider &other) const
    {
        switch (other.m_type)
            {
                case fe::colliderType::AABB:
                    {
                        auto &second = static_cast<const fe::AABB&>(other);
                        // Check if any axis doesnt collide another
                        if (m_max.x + m_position.x < second.m_min.x + second.m_position.x || m_min.x + m_position.x > second.m_max.x + second.m_position.x) return false;
                        if (m_max.y + m_position.y < second.m_min.y + second.m_position.y || m_min.y + m_position.y > second.m_max.y + second.m_position.y) return false;

                        return true;
                    }
                    break;
                default:
                    break;
            }

        return false;
    }

bool fe::AABB::doesContain(const fe::Vector2d &point) const
    {
        return point.x > m_min.x + m_position.x && point.x < m_max.x + m_position.x &&
               point.y > m_min.y + m_position.y && point.y < m_max.y + m_position.y;
    }

bool fe::AABB::doesRayIntersect(const Vector2d &origin, const Vector2d &direction) const
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
                        if (origin[i] < (m_min[i] + m_position[i]) || origin[i] > (m_max[i] + m_position[i])) return false;
                    }
                else
                    {
                        float ood = 1.f / direction[i];
                        float t1 = ((m_min[i] + m_position[i]) - origin[i]) * ood;
                        float t2 = ((m_max[i] + m_position[i]) - origin[i]) * ood;

                        if (t1 > t2)
                            {
                                float temp = t2;
                                t2 = t1;
                                t1 = temp; 
                            }

                        tMin = std::max(tMin, t1);
                        tMax = std::min(tMax, t2);

                        if (tMin > tMax) return false;
                    }
            }

        return true;
    }