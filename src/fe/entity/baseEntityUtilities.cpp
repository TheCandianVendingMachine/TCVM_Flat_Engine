#include <fe/entity/baseEntityUtilities.hpp>
#include <fe/entity/baseEntity.hpp>
#include <fe/subsystems/physics/rigidBody.hpp>

fe::baseEntityUtilities::baseEntityUtilities(baseEntity *entity) :
    m_baseEntity(entity),
    m_slowdownDistance(10.f),
    m_targetPosition(0.f, 0.f),
    m_targeted(false),
    m_arrived(false),
    m_completionRadius(1.f)
    {
    }

void fe::baseEntityUtilities::setSlowdownDistance(float distance)
    {
        m_slowdownDistance = distance;
    }

void fe::baseEntityUtilities::moveToPosition(const fe::Vector2d position)
    {
        moveToPosition(position.x, position.y);
    }

void fe::baseEntityUtilities::moveToPosition(float x, float y)
    {
        m_targetPosition.x = x;
        m_targetPosition.y = y;
        m_targeted = true;
        m_arrived = false;
    }

void fe::baseEntityUtilities::stop()
    {
        if (m_baseEntity->getRigidBody())
            {
                m_targeted = false;
                m_baseEntity->getRigidBody()->setVelocity(0.f, 0.f);
                m_arrived = true;
            }
    }

float fe::baseEntityUtilities::getSlowdownDistance() const
    {
        return m_slowdownDistance;
    }

void fe::baseEntityUtilities::setCompletionRadius(float radius)
    {
        m_completionRadius = radius;
    }

float fe::baseEntityUtilities::getCompletionRadius() const
    {
        return m_completionRadius;
    }

void fe::baseEntityUtilities::update()
    {
        if (m_targeted)
            {
                if (!m_baseEntity->getRigidBody())
                    {
                        m_targeted = false;
                    }
                else
                    {
                        fe::Vector2d targetLine = m_targetPosition - m_baseEntity->getPosition();
                        float distance = std::sqrt(targetLine.x * targetLine.x + targetLine.y * targetLine.y);
                        if (distance > m_completionRadius) // if its less than the completion radius the ent is at the position. Since we round to an integer there wont be any stutter
                            {
                                float maxSpeed = m_baseEntity->getRigidBody()->getMaxVelocity();

                                float possibleSpeed = maxSpeed * (distance / m_slowdownDistance);
                                float speed = std::min(possibleSpeed, maxSpeed);

                                fe::Vector2d desiredVel = targetLine * (speed / distance);
                                fe::Vector2d steeringForce = desiredVel - m_baseEntity->getRigidBody()->getVelocity();

                                m_baseEntity->getRigidBody()->applyForce(steeringForce);
                            }
                        else
                            {
                                m_targeted = false;
                                m_baseEntity->getRigidBody()->setVelocity(0.f, 0.f);
                                //m_baseEntity->setPosition(m_targetPosition);
                                m_arrived = true;
                            }
                    }
            }
    }

bool fe::baseEntityUtilities::getArrived() const
    {
        return m_arrived;
    }

bool fe::baseEntityUtilities::isTargeted() const
    {
        return m_targeted;
    }
