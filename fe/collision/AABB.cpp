#include "AABB.hpp"

fe::AABB::AABB(Vector2d &position, const Vector2d corners[4]) : collider(position)
    {
        m_corners[0] = corners[0];
        m_corners[1] = corners[1];
        m_corners[2] = corners[2];
        m_corners[3] = corners[3];
    }

bool fe::AABB::doesCollide(collider &collider)
    {
        return bool();
    }

bool fe::AABB::containsPoint(const Vector2d &position)
    {
        return position.x > m_position.x && position.x < (m_corners[2].x + m_position.x) &&
               position.y > m_position.y && position.y < (m_corners[2].y + m_position.y);
    }

bool fe::AABB::doesRayCollide(const Vector2d &position, const Vector2d &direction)
    {
        return bool();
    }

bool fe::AABB::doesSegmentCollide(const Vector2d &positionA, const Vector2d &positionB)
    {
        return bool();
    }

fe::Vector2d fe::AABB::getSize()
    {
        return m_corners[2]; // this corner will return the bottom left of the AABB. This represents the size
    }
