// collisionWorld.hpp
// Represents the world the colliders live in
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "collisionBody.hpp"
#include "collisionData.hpp"

namespace fe
    {
        class collisionWorld
            {
                private:
                    fe::poolAllocater<fe::AABB> m_collisionBodies;
                    // Collision data for each collision body. Index is equal to the index of the collision body
                    fe::collisionData m_collisionData[FE_MAX_GAME_OBJECTS];

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void handleCollisions();

                    FLAT_ENGINE_API fe::AABB *createCollider();
                    FLAT_ENGINE_API void deleteCollider(fe::AABB *body);

            };
    }