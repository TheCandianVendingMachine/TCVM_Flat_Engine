// AABB.hpp
// A basic collider type. Allows an object to 
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "collider.hpp"

namespace fe
    {
        class AABB : public collider
            {
                private:
                    // Corners of the AABB
                    Vector2d m_corners[4];

                public:
                    FLAT_ENGINE_API AABB(Vector2d &position, const Vector2d corners[4]);

                    // Returns true if the other collider intersects this collider
                    FLAT_ENGINE_API virtual bool doesCollide(collider &collider);
                    // Returns true if the point is inside the collider
                    FLAT_ENGINE_API virtual bool containsPoint(const Vector2d &position);
                    // Returns true if a ray intersects with this collider
                    FLAT_ENGINE_API virtual bool doesRayCollide(const Vector2d &position, const Vector2d &direction);
                    // Returns true if a line segment intersects this collider
                    FLAT_ENGINE_API virtual bool doesSegmentCollide(const Vector2d &positionA, const Vector2d &positionB);

                    FLAT_ENGINE_API Vector2d getSize();
            };
    }