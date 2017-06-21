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
                    physicsEngine();

                    void startUp();
                    void shutDown();
                    void clear();

                    void setGravity(fe::Vector2d gravity);
                    fe::Vector2d getGravity() const;

                    void simulateForces(float deltaTime);

            };
    }