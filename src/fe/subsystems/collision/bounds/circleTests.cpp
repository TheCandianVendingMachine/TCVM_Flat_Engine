#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/subsystems/collision/bounds/circle.hpp"
#include "fe/math/Vector2.hpp"
#include <cmath>

bool fe::intersects(const fe::circle &a, const fe::circle &b)
    {
        fe::Vector2d dist = fe::lightVector2d(a.m_globalPositionX + a.m_offsetX, a.m_globalPositionY + a.m_offsetY) -
                            fe::lightVector2d(b.m_globalPositionX + b.m_offsetX, b.m_globalPositionY + b.m_offsetY);

        float dist2 = dist.dot(dist); // distance squared
        float radiusSum = a.m_radius + b.m_radius;

        // If the distance^2 < squared radius^2 circles are intersecting
        return dist2 <= (radiusSum * radiusSum);
    }

bool fe::contains(const fe::circle &a, const fe::circle &b)
    {
        fe::Vector2d dist = fe::lightVector2d(a.m_globalPositionX + a.m_offsetX, a.m_globalPositionY + a.m_offsetY) -
                            fe::lightVector2d(b.m_globalPositionX + b.m_offsetX, b.m_globalPositionY + b.m_offsetY);

        float dist2 = dist.dot(dist); // distance squared
        float radiusSum = a.m_radius - b.m_radius;

        // If the distance^2 < aR - bR sphere B is contained within sphere A
        return dist2 <= (radiusSum * radiusSum);
    }

bool fe::contains(const fe::circle &a, float x, float y)
    {
        fe::Vector2d dist(a.m_globalPositionX + a.m_offsetX - x, a.m_globalPositionY + a.m_offsetY - y);
        float dist2 = dist.dot(dist);
        
        return dist2 < (a.m_radius * a.m_radius);
    }

fe::raycastResult fe::rayIntersects(const fe::circle &a, float x, float y, float dirX, float dirY)
    {
        fe::raycastResult result;
        result.m_hit = false;

        fe::Vector2d m = fe::lightVector2d(x, y) - fe::lightVector2d(a.m_globalPositionX + a.m_offsetX, a.m_globalPositionY + a.m_offsetY);
        float b = m.dot(fe::lightVector2d(dirX, dirY));
        float c = m.dot(m) - (a.m_radius * a.m_radius);

        // if the rays origin is outside the sphere and the ray is pointing away from the sphere no collisio
        if (c > 0.f && b > 0.f)
            {
                return result;
            }

        float discr = (b * b) - c;
        if (discr < 0.f) // negative discriminate
            {
                return result;
            }
        result.m_hit = true;
        // quadradic equation to find smallest t value of intersection
        float t = -b - std::sqrt(discr);
        if (t < 0.f)
            {
                // ray started inside sphere
                t = 0.f;
            }

        result.m_positionX = x + t * dirX;
        result.m_positionY = y + t * dirY;

        return result;
    }

fe::raycastResult fe::lineIntersects(const fe::circle &a, float x0, float y0, float x1, float y1)
    {
        fe::raycastResult result;
        result.m_hit = false;

        fe::Vector2d segmentLine(x1 - x0, y1 - y0);
        float segmentLength = segmentLine.magnitude();

        fe::Vector2d m = fe::lightVector2d(x0, y0) - fe::lightVector2d(a.m_globalPositionX + a.m_offsetX, a.m_globalPositionY + a.m_offsetY);
        float b = m.dot(segmentLine / segmentLength);
        float c = m.dot(m) - (a.m_radius * a.m_radius);

        // if the rays origin is outside the sphere and the ray is pointing away from the sphere no collisio
        if (c > 0.f && b > 0.f)
            {
                return result;
            }

        float discr = (b * b) - c;
        if (discr < 0.f) // negative discriminate
            {
                return result;
            }
        // quadradic equation to find smallest t value of intersection
        float t = -b - std::sqrt(discr);

        // 0 <= t <= ||b - a|| for valid intersection
        if (t > segmentLength)
            {
                // lays outside of line segment
                return result;
            }

        if (t < 0.f)
            {
                // ray started inside sphere
                t = 0.f;
            }

        result.m_hit = true;

        result.m_positionX = x0 + t * (segmentLine.x / segmentLength);
        result.m_positionY = y0 + t * (segmentLine.y / segmentLength);

        return result;
    }

fe::Vector2d fe::closestPoint(const fe::circle &a, float x, float y)
    {
        fe::Vector2d center(a.m_globalPositionX + a.m_offsetX, a.m_globalPositionY + a.m_offsetY);
        fe::Vector2d point(x, y);

        fe::Vector2d pc = point - center;

        // c + (r / ||p - c||) * (p - c)
        return center + ((a.m_radius / pc.magnitude()) * pc);
    }
