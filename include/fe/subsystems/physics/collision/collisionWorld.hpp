// collisionWorld.hpp
// Represents the world the colliders live in
#pragma once
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "../../threading/threadJob.hpp"
#include "collisionBody.hpp"
#include "collisionData.hpp"
#include "broadphaseAbstract.hpp"
#include "../../../math/Vector2.hpp"

namespace fe
    {
        class collisionWorld
            {
                private:
                    fe::poolAllocater<fe::collider> m_collisionBodies;
                    struct
                        {
                            fe::lightVector2d position;
                            fe::str event;
                        } m_pointCollision[512];
                    unsigned int m_maxPointIndex;

                    struct 
                        {
                            fe::collider *collider;
                            fe::collisionData data;
                        } m_collisionPairs[FE_MAX_GAME_OBJECTS * 2];
                    unsigned int m_collisionPairIndex;

                    FLAT_ENGINE_API void handleCollision(fe::collider *a, fe::collider *b);
                    FLAT_ENGINE_API void handleCollision(void *leftCollider, void *rightCollider);
                    FLAT_ENGINE_API void handleCollision(void *collider, fe::str event); // point collision

                    FLAT_ENGINE_API void cullCollisionPairs();
                    FLAT_ENGINE_API void handleCollisionPairs();
                    FLAT_ENGINE_API void handlePointCollisions(const fe::broadphaseAbstract *broadphase);
                    FLAT_ENGINE_API void handlePointCollisions(const fe::broadphaseAbstract *broadphase0, const fe::broadphaseAbstract *broadphase1);

                public:
                    FLAT_ENGINE_API collisionWorld();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    // checks for and collides with any collider at the point. Does not collide instantly; it collides at next collision check
                    FLAT_ENGINE_API void queryPoint(fe::lightVector2d point, fe::str event);
                    // checks for and collides with any collider at the point. Does not collide instantly; it collides at next collision check
                    FLAT_ENGINE_API void queryPoint(fe::Vector2d point, fe::str event);
                    // checks for and collides with any collider at the point. Does not collide instantly; it collides at next collision check
                    FLAT_ENGINE_API void queryPoint(float x, float y, fe::str event);

                    FLAT_ENGINE_API void handleCollisions(const fe::broadphaseAbstract *broadphase);
                    FLAT_ENGINE_API void handleCollisions(const fe::broadphaseAbstract *broadphaseDynamic, const fe::broadphaseAbstract *broadphaseStatic);

                    FLAT_ENGINE_API fe::collider *createCollider(float sizeX, float sizeY);
                    FLAT_ENGINE_API void deleteCollider(fe::collider *body);

            };
    }