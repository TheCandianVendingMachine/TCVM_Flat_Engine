// collisionWorld.hpp
// Represents the world the colliders live in
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../memory/poolAllocater.hpp"
#include "collisionBody.hpp"

namespace fe
    {
        class collisionWorld
            {
                private:
                    fe::poolAllocater<fe::collisionBody> m_rigidBodies;

                public:
                    void startUp();
                    void shutDown();
                    void clear();

                    void handleCollisions();

                    fe::collisionBody *createCollider();
                    void deleteCollider(fe::collisionBody *body);

            };
    }