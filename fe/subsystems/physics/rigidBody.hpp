// rigidBody.hpp
// Simple rigid body. Can move
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../math/Vector2.hpp"
#include "transformable.hpp"

namespace fe
    {
        class rigidBody : public fe::transformable
            {
                private:
                    // Current velocity of the object during this frame
                    float m_velocityX;
                    float m_velocityY;
                    // Current force being applied next frame
                    float m_forceX;
                    float m_forceY;

                    // Max speed object can go
                    float m_maxSpeed;
                    // How heavy the object is
                    float m_mass;
                    // How much the object will be affected by friction
                    float m_frictionCoeff; // unit float for friction. 1 = Stops immediately -- 0 = Never Slows Down

                    // if the object will be updated or not
                    bool m_enabled;

                public:
                    FLAT_ENGINE_API rigidBody();
                    FLAT_ENGINE_API rigidBody(float mass);
                    FLAT_ENGINE_API rigidBody(float mass, float maxSpeed);
                    FLAT_ENGINE_API rigidBody(float mass, float maxSpeed, float frictionCoeff);

                    FLAT_ENGINE_API void enable(bool value);
                    FLAT_ENGINE_API bool getEnabled() const;

                    FLAT_ENGINE_API void setMass(float newMass);
                    FLAT_ENGINE_API void setMaxSpeed(float maxSpeed);
                    FLAT_ENGINE_API void setFrictionCoefficient(float fricCoeff);

                    // applies a force upon the rigid body. 
                    FLAT_ENGINE_API void applyForce(float x, float y);

                    FLAT_ENGINE_API void setVelocity(float x, float y);
                    FLAT_ENGINE_API void setForce(float x, float y);
                    FLAT_ENGINE_API void setDirection(float x, float y);

                    FLAT_ENGINE_API fe::lightVector2d getVelocity() const;
                    FLAT_ENGINE_API fe::lightVector2d getForce() const;
                    FLAT_ENGINE_API fe::lightVector2d getDirection() const;

                    FLAT_ENGINE_API float getSpeed() const;
                    FLAT_ENGINE_API float getTotalForce() const;
                    FLAT_ENGINE_API float getHeading() const;

                    FLAT_ENGINE_API float getMass() const;
                    FLAT_ENGINE_API float getMaxVelocity() const;
                    FLAT_ENGINE_API float getFrictionCoefficient() const;

                    // Updates position of object based on force
                    FLAT_ENGINE_API void update(float acellX, float acellY, float deltaTime);

            };
    }