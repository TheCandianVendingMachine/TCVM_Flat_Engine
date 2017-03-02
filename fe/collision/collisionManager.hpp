// collisionManager.hpp
// manages all collision in game. Has functions to detect collision between colliders
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "colliders.hpp"

namespace fe
    {
        // Collision functions
        FLAT_ENGINE_API bool doesCollide(const AABB &first, const AABB &second);

        // Point-in-collider functions
        FLAT_ENGINE_API bool doesContain(const AABB &first, const Vector2d &point);

        // Ray-Collider functions
        FLAT_ENGINE_API bool doesRayIntersect(AABB &first, Vector2d &origin, Vector2d &direction);

    }