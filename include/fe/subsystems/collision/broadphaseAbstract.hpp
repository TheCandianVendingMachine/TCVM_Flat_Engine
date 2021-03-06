// broadphaseAbstract.hpp
// An abstract class to serve for the collision engine's broadphase collision detection
#pragma once
#include "fe/flatEngineExport.hpp"

#include <list>
#include <utility>
#include <functional>
#include "raycastResult.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/typeDefines.hpp"

namespace fe
    {
        struct collider;
        struct AABB;
        struct circle;
        class broadphaseAbstract
            {
                protected:
                    bool m_debug = false;

                public:
                    virtual void startUp() {}

                    // Adds an AABB to the broadphase algorithm
                    virtual void add(fe::collider *collider) = 0;
                    // Removes the collider from the broadphase algorithm
                    virtual void remove(fe::collider *collider) = 0;

                    // Updates all colliders in the broadphase algorithm
                    virtual void update(fe::collider *collider) = 0;

                    // Returns user data containing what is needed to resolve the query
                    virtual void colliderAABB(fe::AABB &testAABB, std::function<void(fe::collider*)> callback) const = 0;

                    // Runs callback function on all colliders that get hit by the circle
                    virtual void colliderCircle(fe::circle &testCircle, std::function<void(fe::collider*)> callback) const = 0;

                    // Returns the collider that is at the point
                    virtual void *colliderAtPoint(float x, float y) const = 0;
                    // Returns the collider that is at the point
                    inline void *colliderAtPoint(const fe::Vector2d &point) const { return colliderAtPoint(point.x, point.y); }
                    // Returns the collider that is at the point
                    inline void *colliderAtPoint(const fe::lightVector2d &point) const { return colliderAtPoint(point.x, point.y); }

                    // Casts a ray and tests against the broadphase algorithm
                    virtual raycastResult raycast(float x, float y, float dirX, float dirY, fe::str ignoreGroup) const = 0;
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult raycast(const fe::Vector2d &origin, const fe::Vector2d &direction, fe::str ignoreGroup) const { return raycast(origin.x, origin.y, direction.x, direction.y, ignoreGroup); }
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult raycast(const fe::lightVector2d &origin, const fe::lightVector2d &direction, fe::str ignoreGroup) const { return raycast(origin.x, origin.y, direction.x, direction.y, ignoreGroup); }

                    // Casts a ray and tests against the broadphase algorithm
                    virtual raycastResult linecast(float x0, float y0, float x1, float y1, fe::str ignoreGroup) const = 0;
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult linecast(const fe::Vector2d &start, const fe::Vector2d &end, fe::str ignoreGroup) const { return linecast(start.x, start.y, end.x, end.y, ignoreGroup); }
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult linecast(const fe::lightVector2d &start, const fe::lightVector2d &end, fe::str ignoreGroup) const { return linecast(start.x, start.y, end.x, end.y, ignoreGroup); }

                    virtual void debugDraw() {}

                    // Toggles debug drawing
                    FLAT_ENGINE_API void debugMode(bool value);

            };
    }