// rigidBody.hpp
// Simple rigid body. Can move
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../math/Vector2.hpp"
#include "../serializer/serializable.hpp"
#include "transformable.hpp"

namespace fe
    {
        class rigidBody : public fe::serializable
            {
                private:
                    void *m_metaData; // meta data of the collider. Usually the entity owner

                    // Positions of the entity
                    float m_positionX;
                    float m_positionY;

                    // old positions of the entitiy. Used for optimizaitons regarding collision world
                    float m_oldPositionX;
                    float m_oldPositionY;

                    // Current velocity of the object during this frame
                    float m_velocityX;
                    float m_velocityY;
                    // Current force
                    float m_forceX;
                    float m_forceY;
                    // Last force that was applied
                    float m_impulseX;
                    float m_impulseY;

                    // Max speed object can go
                    float m_maxSpeed;
                    // How heavy the object is
                    float m_mass;
                    // How much the object will be affected by friction
                    float m_frictionCoeff;

                    // Normal forces acting upon this entity
                    float m_normalForceX;
                    float m_normalForceY;

                    // if the object will be updated or not
                    bool m_enabled;

                    friend class baseEntity;

                public:
                    FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                    FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

                    FLAT_ENGINE_API float getVelocityX() const;
                    FLAT_ENGINE_API float getVelocityY() const;
                    FLAT_ENGINE_API float getForceX() const;
                    FLAT_ENGINE_API float getForceY() const;
                    FLAT_ENGINE_API float getImpulseX() const;
                    FLAT_ENGINE_API float getImpulseY() const;
                    FLAT_ENGINE_API float getPositionX() const;
                    FLAT_ENGINE_API float getPositionY() const;
                    FLAT_ENGINE_API float getOldPositionX() const;
                    FLAT_ENGINE_API float getOldPositionY() const;

                    FLAT_ENGINE_API float getNormalForceX() const;
                    FLAT_ENGINE_API float getNormalForceY() const;

                    FLAT_ENGINE_API float getSpeed() const;
                    FLAT_ENGINE_API float getTotalForce() const;
                    FLAT_ENGINE_API float getHeading() const;

                    FLAT_ENGINE_API float getMass() const;
                    FLAT_ENGINE_API float getMaxVelocity() const;
                    FLAT_ENGINE_API float getFrictionCoefficient() const;

                    FLAT_ENGINE_API fe::Vector2d getVelocity() const;
                    FLAT_ENGINE_API fe::Vector2d getForce() const;
                    FLAT_ENGINE_API fe::Vector2d getDirection() const;
                    FLAT_ENGINE_API fe::Vector2d getPosition() const;

                    FLAT_ENGINE_API void setMass(float newMass);
                    FLAT_ENGINE_API void setMaxSpeed(float maxSpeed);
                    FLAT_ENGINE_API void setFrictionCoefficient(float fricCoeff);

                    FLAT_ENGINE_API void applyNormalForce(float x, float y);

                    // applies a force upon the rigid body. 
                    FLAT_ENGINE_API void applyForce(float x, float y);
                    FLAT_ENGINE_API void applyForce(fe::lightVector2d force);

                    FLAT_ENGINE_API void setVelocity(float x, float y);
                    FLAT_ENGINE_API void setVelocity(fe::lightVector2d velocity);

                    FLAT_ENGINE_API void setForce(float x, float y);
                    FLAT_ENGINE_API void setForce(fe::lightVector2d force);

                    FLAT_ENGINE_API void setDirection(float x, float y);
                    FLAT_ENGINE_API void setDirection(fe::lightVector2d direction);

                    FLAT_ENGINE_API void setPosition(float x, float y);
                    FLAT_ENGINE_API void setPosition(fe::lightVector2d position);

                    FLAT_ENGINE_API rigidBody();
                    FLAT_ENGINE_API rigidBody(float mass);
                    FLAT_ENGINE_API rigidBody(float mass, float maxSpeed);
                    FLAT_ENGINE_API rigidBody(float mass, float maxSpeed, float frictionCoeff);

                    FLAT_ENGINE_API void enable(bool value);
                    FLAT_ENGINE_API bool getEnabled() const;

                    FLAT_ENGINE_API void setMetaData(void *data);
                    FLAT_ENGINE_API void *getMetaData() const;

                    // Updates position of object based on force
                    inline void update(float deltaTime)
                        {
                            m_velocityX += (m_impulseX / m_mass) * deltaTime;
                            m_velocityY += (m_impulseY / m_mass) * deltaTime;

                            if (m_maxSpeed * m_maxSpeed < m_velocityX * m_velocityX)
                                {
                                    m_velocityX *= std::sqrt((m_maxSpeed * m_maxSpeed) / (m_velocityX * m_velocityX));
                                }

                            if (m_maxSpeed * m_maxSpeed < m_velocityY * m_velocityY)
                                {
                                    m_velocityY *= std::sqrt((m_maxSpeed * m_maxSpeed) / (m_velocityY * m_velocityY));
                                }

                            m_oldPositionX = m_positionX;
                            m_oldPositionY = m_positionY;

                            m_positionX += m_velocityX * deltaTime;
                            m_positionY += m_velocityY * deltaTime;

                            m_forceX += m_impulseX;
                            m_forceY += m_impulseY;

                            m_impulseX = 0.f;
                            m_impulseY = 0.f;

                            m_normalForceX = 0.f;
                            m_normalForceY = 0.f;
                        }

            };
    }