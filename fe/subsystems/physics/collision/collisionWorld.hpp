// collisionWorld.hpp
// Represents the world the colliders live in
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "collisionBody.hpp"
#include "collisionData.hpp"
#include "broadphaseAbstract.hpp"

namespace fe
    {
        class collisionWorld
            {
                private:
                    fe::poolAllocater<fe::AABB> m_collisionBodies;
                    fe::broadphaseAbstract *m_broadphase;

                    FLAT_ENGINE_API void handleCollision(fe::collider *a, fe::collider *b);

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void setBroadphase(fe::broadphaseAbstract *broadphase);
                    FLAT_ENGINE_API fe::broadphaseAbstract *getBroadphase() const;

                    FLAT_ENGINE_API void handleCollisions();

                    FLAT_ENGINE_API fe::AABB *createCollider();
                    FLAT_ENGINE_API void deleteCollider(fe::AABB *body);

            };
    }