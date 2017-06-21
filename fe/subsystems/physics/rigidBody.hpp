// rigidBody.hpp
// Simple rigid body. Can move
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

#include "transformable.hpp"
#include "../../math/Vector2.hpp"

namespace fe
    {
        class rigidBody : public fe::transformable
            {
                private:
                    // Current velocity of the object during this frame
                    fe::Vector2d m_velocity;
                    // Current force being applied next frame
                    fe::Vector2d m_force;

                    // Max speed object can go
                    float m_maxSpeed;
                    // How heavy the object is
                    float m_mass;
                    // How much the object will be affected by friction
                    float m_frictionCoeff; // unit float for friction. 1 = Stops immediately -- 0 = Never Slows Down

                public:
                    rigidBody(float mass);
                    rigidBody(float mass, float maxSpeed);
                    rigidBody(float mass, float maxSpeed, float frictionCoeff);

                    void setMass(float newMass);
                    void setMaxSpeed(float maxSpeed);
                    void setFrictionCoefficient(float fricCoeff);

                    // applies a force upon the rigid body. 
                    void applyForce(fe::Vector2d force);

                    void setVelocity(fe::Vector2d newVel);
                    void setForce(fe::Vector2d newForce);
                    void setDirection(fe::Vector2d newDirection);

                    fe::Vector2d getVelocity() const;
                    fe::Vector2d getForce() const;
                    fe::Vector2d getDirection() const;

                    float getSpeed() const;
                    float getTotalForce() const;
                    float getHeading() const;

                    float getMass() const;
                    float getMaxVelocity() const;
                    float getFrictionCoefficient() const;

                    // Updates position of object based on force
                    void update(fe::Vector2d accel, float deltaTime);

            };
    }