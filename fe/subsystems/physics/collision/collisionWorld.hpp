// collisionWorld.hpp
// Represents the world the colliders live in
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "../../threading/threadJob.hpp"
#include "collisionBody.hpp"
#include "collisionData.hpp"
#include "broadphaseAbstract.hpp"

namespace fe
    {
        class collisionWorld
            {
                private:
                    fe::poolAllocater<fe::collider> m_collisionBodies;

                    FLAT_ENGINE_API void handleCollision(fe::collider *a, fe::collider *b);
                    FLAT_ENGINE_API void handleCollision(void *leftCollider, void *rightCollider);

                public:
                    FLAT_ENGINE_API collisionWorld();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void handleCollisions(const fe::broadphaseAbstract *broadphase);

                    FLAT_ENGINE_API fe::collider *createCollider(float sizeX, float sizeY);
                    FLAT_ENGINE_API void deleteCollider(fe::collider *body);

            };
    }