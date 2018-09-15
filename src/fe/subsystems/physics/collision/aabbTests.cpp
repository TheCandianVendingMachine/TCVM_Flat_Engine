#include "fe/subsystems/physics/collision/aabbTests.hpp"
#include "fe/subsystems/physics/collision/collisionBounds.hpp"
#include "fe/typeDefines.hpp"
#include "fe/math/Vector2.hpp"
#include <limits>
#include <algorithm>

bool fe::intersects(const AABB &a, AABB &b)
    {
        return  !((a.m_globalPositionX + a.m_sizeX < b.m_globalPositionX || a.m_globalPositionX > b.m_globalPositionX + b.m_sizeX) ||
                  (a.m_globalPositionY + a.m_sizeY < b.m_globalPositionY || a.m_globalPositionY > b.m_globalPositionY + b.m_sizeY));
    }

bool fe::contains(const AABB &a, const AABB &b)
    {
        return  (b.m_globalPositionX >= a.m_globalPositionX && b.m_globalPositionX + b.m_sizeX <= a.m_globalPositionX + a.m_sizeX) &&
                (b.m_globalPositionY >= a.m_globalPositionY && b.m_globalPositionY + b.m_sizeY <= a.m_globalPositionY + a.m_sizeY);
    }

bool fe::contains(const AABB &a, float x, float y)
    {
        return x >= a.m_globalPositionX && y >= a.m_globalPositionY && x < a.m_globalPositionX + a.m_sizeX && y < a.m_globalPositionY + a.m_sizeY;
    }

fe::raycastResult fe::rayIntersects(const AABB &a, float x, float y, float dirX, float dirY)
    {
        fe::raycastResult result;
        result.m_hit = false;

        constexpr float epsilon = fe::FE_EPSILON;

        float min = 0.f;
        float max = std::numeric_limits<float>::max();

        fe::lightVector2d aMin(a.m_globalPositionX, a.m_globalPositionY);
        fe::lightVector2d aMax(a.m_globalPositionX + a.m_sizeX, a.m_globalPositionY + a.m_sizeY);

        // test X slab
        if (std::abs(dirX) < epsilon)
            {
                if (x < aMin.x || x > aMax.x)
                    {
                        return result;
                    }
            }
        else
            {
                float ood = 1.f / dirX;
                float t1 = (aMin.x - x) * ood;
                float t2 = (aMax.x - x) * ood;

                if (t1 > t2)
                    {
                        float temp = t1;
                        t1 = t2;
                        t2 = temp;
                    }

                min = std::max(min, t1);
                max = std::min(max, t2);

                if (min > max)
                    {
                        return result;
                    }
            }

        // test Y slab
        if (std::abs(dirY) < epsilon)
            {
                if (y < aMin.y || y > aMax.y)
                    {
                        return result;
                    }
            }
        else
            {
                float ood = 1.f / dirY;
                float t1 = (aMin.y - y) * ood;
                float t2 = (aMax.y - y) * ood;

                if (t1 > t2)
                    {
                        float temp = t1;
                        t1 = t2;
                        t2 = temp;
                    }

                min = std::max(min, t1);
                max = std::min(max, t2);

                if (min > max)
                    {
                        return result;
                    }
            }


        result.m_hit = true;
        result.m_positionX = x + dirX * min;
        result.m_positionY = y + dirY * min;
        return result;
    }

fe::raycastResult fe::lineIntersects(const AABB &a, float x0, float y0, float x1, float y1)
    {
        fe::raycastResult result;
        result.m_hit = false;

        constexpr float epsilon = fe::FE_EPSILON;

        fe::lightVector2d aMin(a.m_globalPositionX, a.m_globalPositionY);
        fe::lightVector2d aMax(a.m_globalPositionX + a.m_sizeX, a.m_globalPositionY + a.m_sizeY);

        fe::lightVector2d extent = aMax - aMin;
        fe::lightVector2d lineMid = fe::lightVector2d(x0, y0) + fe::lightVector2d(x1, y1) - aMin - aMax;
        fe::lightVector2d dir = fe::lightVector2d(x1, y1) - fe::lightVector2d(x0, y0);

        float adx = std::abs(dir.x);
        float ady = std::abs(dir.y);
        if (std::abs(lineMid.x) > extent.x + adx || 
            std::abs(lineMid.y) > extent.y + ady)
            {
                return result;
            }

        adx += epsilon;
        ady += epsilon;

        if (std::abs(lineMid.x * dir.y - lineMid.y * dir.x) > extent.x * ady + extent.y * adx)
            {
                return result;
            }

        result.m_hit = true;
        result.m_positionX = 0.f;
        result.m_positionY = 0.f;
        return result;
    }
