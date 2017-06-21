// physicsEngine.hpp
// handles all physics.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

#include "rigidBody.hpp"
#include "../memory/poolAllocater.hpp"

namespace fe
    {
        class physicsEngine
            {
                private:
                    fe::poolAllocater<rigidBody> m_rigidBodies;
                    fe::Vector2d m_gravityForce;

                public:
                    FLAT_ENGINE_API physicsEngine();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void setGravity(fe::Vector2d gravity);
                    FLAT_ENGINE_API fe::Vector2d getGravity() const;

                    FLAT_ENGINE_API void simulateForces(float deltaTime);

            };
    }