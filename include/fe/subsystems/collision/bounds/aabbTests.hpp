// aabbTests.hpp
// All possible collision tests against an AABB
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/collision/raycastResult.hpp"
#include "fe/math/Vector2.hpp"

namespace fe
    {
        struct AABB;
        // Returns if B intersects A
        FLAT_ENGINE_API bool intersects(const fe::AABB &a, const fe::AABB &b);
        // Returns if B is contained within A
        FLAT_ENGINE_API bool contains(const fe::AABB &a, const fe::AABB &b);
        // Returns if point is contained within A
        FLAT_ENGINE_API bool contains(const fe::AABB &a, float x, float y);
        // Returns data on ray with origin (x,y) and direction (dirX,dirY) intersect with AABB
        FLAT_ENGINE_API fe::raycastResult rayIntersects(const fe::AABB &a, float x, float y, float dirX, float dirY);
        // Returns data on line segment with origin (x0,y0) and final point (x1,y1) intersect with AABB
        FLAT_ENGINE_API fe::raycastResult lineIntersects(const fe::AABB &a, float x0, float y0, float x1, float y1);
        // Returns the closest point on an AABB to the coordinate (x,y)
        FLAT_ENGINE_API fe::Vector2d closestPoint(const fe::AABB &a, float x, float y);
    }
