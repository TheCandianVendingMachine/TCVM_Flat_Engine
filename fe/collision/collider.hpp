// collider.hpp
// a base class for all colliders
#pragma once
#include "../math/Vector2.hpp"

namespace fe
    {
        class collider
            {
                protected:
                        // The center point of an object
                        Vector2d &m_position;

                public:
                    // Position is the center point of the object.
                    collider(Vector2d &position) : m_position(position) {}

                    const Vector2d &getPosition() { return m_position; }

                    // Returns true if the other collider intersects this collider
                    virtual bool doesCollide(collider &collider) = 0;
                    // Returns true if the point is inside the collider
                    virtual bool containsPoint(const Vector2d &position) = 0;
                    // Returns true if a ray intersects with this collider
                    virtual bool doesRayCollide(const Vector2d &position, const Vector2d &direction) = 0;
                    // Returns true if a line segment intersects this collider
                    virtual bool doesSegmentCollide(const Vector2d &positionA, const Vector2d &positionB) = 0;
            };
    }