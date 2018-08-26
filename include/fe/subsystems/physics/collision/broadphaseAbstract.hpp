// broadphaseAbstract.hpp
// An abstract class to serve for the collision engine's broadphase collision detection
#pragma once
#include "../../../flatEngineExport.hpp"

#include <list>
#include <utility>
#include <functional>
#include "raycastResult.hpp"
#include "../../../math/Vector2.hpp"
#include "../../../typeDefines.hpp"

namespace fe
    {
        struct collider;
        struct AABB;
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
                    virtual void colliderAABB(fe::AABB &testAABB, std::function<void(void*)> callback) const = 0;

                    // Returns the collider that is at the point
                    virtual void *colliderAtPoint(float x, float y) const = 0;
                    // Returns the collider that is at the point
                    inline void *colliderAtPoint(const fe::Vector2d &point) const { return colliderAtPoint(point.x, point.y); }
                    // Returns the collider that is at the point
                    inline void *colliderAtPoint(const fe::lightVector2d &point) const { return colliderAtPoint(point.x, point.y); }

                    // Casts a ray and tests against the broadphase algorithm
                    virtual raycastResult raycast(float x, float y, float dirX, float dirY) const = 0;
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult raycast(fe::Vector2d &origin, fe::Vector2d &direction) const { return raycast(origin.x, origin.y, direction.x, direction.y); }
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult raycast(fe::lightVector2d &origin, fe::lightVector2d &direction) const { return raycast(origin.x, origin.y, direction.x, direction.y); }

                    virtual void debugDraw() {}

                    // Toggles debug drawing
                    FLAT_ENGINE_API void debugMode(bool value);

            };
    }