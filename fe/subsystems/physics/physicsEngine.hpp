// physicsEngine.hpp
// handles all physics.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "../threading/threadJob.hpp"
#include "rigidBody.hpp"
#include "../memory/poolAllocater.hpp"

namespace fe
    {
        class physicsEngine
            {
                private:
                    fe::poolAllocater<rigidBody> m_rigidBodies;
                    float m_gravityForceX;
                    float m_gravityForceY;

                    struct physicsJob : public fe::threadJob
                        {
                            fe::poolAllocater<rigidBody> &m_rigidBodies;

                            float m_gravityX;
                            float m_gravityY;
                            float m_deltaTime;
                            unsigned int m_iterations;
                            unsigned int m_initialIndex;
                            unsigned int m_endIndex;

                            FLAT_ENGINE_API physicsJob(fe::poolAllocater<rigidBody> &rigidBodies, float gravityX, float gravityY);
                            FLAT_ENGINE_API bool execute();
                        };

                    physicsJob jobA;
                    physicsJob jobB;
                    physicsJob jobC;
                    physicsJob jobD;

                public:
                    FLAT_ENGINE_API physicsEngine();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void setGravity(fe::Vector2d gravity);
                    FLAT_ENGINE_API fe::Vector2d getGravity() const;

                    // Simulates forces on all objects the amount of iterations it takes the fixed time step to complete
                    FLAT_ENGINE_API void preUpdate(float deltaTime, unsigned int iterations);
                    // Waits for the physics jobs to be completed
                    FLAT_ENGINE_API void simulateForces();

                    FLAT_ENGINE_API fe::rigidBody *createRigidBody();
                    FLAT_ENGINE_API void deleteRigidBody(fe::rigidBody *body);

            };
    }