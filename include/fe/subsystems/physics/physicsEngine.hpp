// physicsEngine.hpp
// handles all physics.
#pragma once
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "../threading/threadJob.hpp"
#include "rigidBody.hpp"
#include "../memory/poolAllocater.hpp"
#include "../../math/Vector3.hpp"
#include "../messaging/eventHandler.hpp"
#include "collision/collisionData.hpp"

namespace fe
    {
        struct collider;

        class physicsEngine : public fe::eventHandler
            {
                private:
                    fe::poolAllocater<rigidBody> m_rigidBodies;

                    float m_gravityForceX;
                    float m_gravityForceY;
                    float m_gravityForceZ;

                    // How much velocity is taken away with friction calculation
                    float m_frictionImpulse;

                    const unsigned int m_maxObjectsUntilThread;

                    FLAT_ENGINE_API void handleCollision(fe::collider *a, fe::collider *b, fe::collisionData aData, fe::collisionData bData);

                public:
                    FLAT_ENGINE_API physicsEngine();

                    FLAT_ENGINE_API void handleEvent(const gameEvent &event);

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void setGravity(fe::Vector3d gravity);
                    FLAT_ENGINE_API void setGravityX(float gravity);
                    FLAT_ENGINE_API void setGravityY(float gravity);
                    FLAT_ENGINE_API void setGravityZ(float gravity);
                    FLAT_ENGINE_API fe::Vector3d getGravity() const;

                    FLAT_ENGINE_API void setFrictionImpulse(float friction);

                    // Simulates forces on all objects the amount of iterations it takes the fixed time step to complete
                    FLAT_ENGINE_API void preUpdate(float deltaTime, unsigned int iterations);
                    // Waits for the physics jobs to be completed
                    FLAT_ENGINE_API void simulateForces(float deltaTime, unsigned int iterations);

                    FLAT_ENGINE_API fe::rigidBody *createRigidBody();
                    FLAT_ENGINE_API void deleteRigidBody(fe::rigidBody *body);

            };
    }