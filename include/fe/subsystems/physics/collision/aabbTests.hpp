// aabbTests.hpp
// All possible collision tests again an AABB
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/physics/collision/raycastResult.hpp"

namespace fe
    {
        struct AABB;
        // Returns if B intersects A
        FLAT_ENGINE_API bool intersects(const AABB &a, AABB &b);
        // Returns if B is contained within A
        FLAT_ENGINE_API bool contains(const AABB &a, const AABB &b);
        // Returns if point is contained within A
        FLAT_ENGINE_API bool contains(const AABB &a, float x, float y);
        // Returns data on ray with origin (x,y) and direction (dirX,dirY) intersect with AABB
        FLAT_ENGINE_API fe::raycastResult rayIntersects(const AABB &a, float x, float y, float dirX, float dirY);
        // Returns data on line segment with origin (x0,y0) and final point (x1,y1) intersect with AABB
        FLAT_ENGINE_API fe::raycastResult lineIntersects(const AABB &a, float x0, float y0, float x1, float y1);
    }
