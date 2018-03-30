// physicsEngine.hpp
// handles all physics.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "../threading/threadJob.hpp"
#include "rigidBody.hpp"
#include "../memory/poolAllocater.hpp"
#include "../../math/Vector3.hpp"

namespace fe
    {
        class physicsEngine
            {
                private:
                    fe::poolAllocater<rigidBody> m_rigidBodies;

                    float m_gravityForceX;
                    float m_gravityForceY;
                    float m_gravityForceZ;

                    const unsigned int m_maxObjectsUntilThread;

                public:
                    FLAT_ENGINE_API physicsEngine();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void setGravity(fe::Vector3d gravity);
                    FLAT_ENGINE_API fe::Vector3d getGravity() const;

                    // Simulates forces on all objects the amount of iterations it takes the fixed time step to complete
                    FLAT_ENGINE_API void preUpdate(float deltaTime, unsigned int iterations);
                    // Waits for the physics jobs to be completed
                    FLAT_ENGINE_API void simulateForces(float deltaTime, unsigned int iterations);

                    FLAT_ENGINE_API fe::rigidBody *createRigidBody();
                    FLAT_ENGINE_API void deleteRigidBody(fe::rigidBody *body);

            };
    }