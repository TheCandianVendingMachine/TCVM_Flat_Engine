// broadphaseAbstract.hpp
// An abstract class to serve for the collision engine's broadphase collision detection
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

#include <list>
#include <utility>
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
                    bool m_debug;

                public:
                    // Adds an AABB to the broadphase algorithm
                    virtual void add(fe::collider *collider) = 0;
                    // Removes the collider from the broadphase algorithm
                    virtual void remove(fe::collider *collider) = 0;

                    // Updates all colliders in the broadphase algorithm
                    virtual void update(float dt) = 0;

                    // Returns a list of the colliders that are possibly intersecting
                    virtual const std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int> computeColliderPairs() = 0;

                    // Returns the collider that is at the point
                    virtual collider *colliderAtPoint(float x, float y) = 0;
                    // Returns the collider that is at the point
                    inline collider *colliderAtPoint(fe::Vector2d &point) { return colliderAtPoint(point.x, point.y); }
                    // Returns the collider that is at the point
                    inline collider *colliderAtPoint(fe::lightVector2d &point) { return colliderAtPoint(point.x, point.y); }

                    // Returns an array of objects that collide with the AABB argument.
                    // First return value is how many objects collide
                    // Second return value is the array of objects that have collided
                    virtual std::pair<unsigned int, collider*[FE_MAX_GAME_OBJECTS]> collidesWithAABB(fe::collider &aabb) = 0;

                    // Casts a ray and tests against the broadphase algorithm
                    virtual raycastResult raycast(float x, float y, float dirX, float dirY) = 0;
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult raycast(fe::Vector2d &origin, fe::Vector2d &direction) { return raycast(origin.x, origin.y, direction.x, direction.y); }
                    // Casts a ray and tests against the broadphase algorithm
                    inline raycastResult raycast(fe::lightVector2d &origin, fe::lightVector2d &direction) { return raycast(origin.x, origin.y, direction.x, direction.y); }

                    // Toggles debug drawing
                    FLAT_ENGINE_API void debugMode(bool value);

            };
    }