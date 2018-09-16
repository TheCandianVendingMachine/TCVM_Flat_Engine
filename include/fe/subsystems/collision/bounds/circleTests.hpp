// circleTests.hpp
// All possible collision tests against a circle
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/collision/raycastResult.hpp"

namespace fe
{
    struct circle;
    // Returns if B intersects A
    FLAT_ENGINE_API bool intersects(const fe::circle &a, const fe::circle &b);
    // Returns if B is contained within A
    FLAT_ENGINE_API bool contains(const fe::circle &a, const fe::circle &b);
    // Returns if point is contained within A
    FLAT_ENGINE_API bool contains(const fe::circle &a, float x, float y);
    // Returns data on ray with origin (x,y) and direction (dirX,dirY) intersect with cirlce
    FLAT_ENGINE_API fe::raycastResult rayIntersects(const fe::circle &a, float x, float y, float dirX, float dirY);
    // Returns data on line segment with origin (x0,y0) and final point (x1,y1) intersect with circle
    FLAT_ENGINE_API fe::raycastResult lineIntersects(const fe::circle &a, float x0, float y0, float x1, float y1);
}
