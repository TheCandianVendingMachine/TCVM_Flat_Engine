#include "fe/subsystems/collision/bounds/mixedTests.hpp"
#include "fe/subsystems/collision/bounds/aabbTests.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/subsystems/collision/bounds/AABB.hpp"
#include "fe/subsystems/collision/bounds/circle.hpp"
#include "fe/math/Vector2.hpp"

bool fe::intersects(const fe::AABB &a, const fe::circle &b)
    {
        fe::Vector2d point = fe::closestPoint(a, b.m_globalPositionX, b.m_globalPositionY);

        fe::Vector2d v = point - fe::Vector2d(a.m_globalPositionX, a.m_globalPositionY);
        return v.dot(v) <= (b.m_radius * b.m_radius);
    }

bool fe::intersects(const fe::circle &a, const fe::AABB &b)
    {
        return intersects(b, a);
    }
