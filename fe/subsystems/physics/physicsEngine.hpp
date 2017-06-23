// physicsEngine.hpp
// handles all physics.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "rigidBody.hpp"
#include "../memory/poolAllocater.hpp"

namespace fe
    {
        class physicsEngine
            {
                private:
                    fe::poolAllocater<rigidBody> m_rigidBodies;
                    /*struct rigidBodyData 
                        {
                            fe::Vector2d accelForce;
                            fe::Vector2d frictionForce;
                            fe::rigidBody *body = nullptr;
                        } //m_rigidBodyData[FE_MAX_GAME_OBJECTS];*/

                    fe::Vector2d m_gravityForce;

                public:
                    FLAT_ENGINE_API physicsEngine();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void setGravity(fe::Vector2d gravity);
                    FLAT_ENGINE_API fe::Vector2d getGravity() const;

                    FLAT_ENGINE_API void preUpdate();
                    // Simulates forces on all objects the amount of iterations it takes the fixed time step to complete
                    FLAT_ENGINE_API void simulateForces(float deltaTime, unsigned int iterations);

                    FLAT_ENGINE_API fe::rigidBody *createRigidBody();
                    FLAT_ENGINE_API void deleteRigidBody(fe::rigidBody *body);

            };
    }