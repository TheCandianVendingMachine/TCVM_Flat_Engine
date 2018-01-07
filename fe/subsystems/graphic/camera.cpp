#include "camera.hpp"
#include <cmath>

fe::camera::camera() : 
    m_view({0.f, 0.f}, {0.f, 0.f}),
    m_position(0.f, 0.f),
    m_size(0.f, 0.f),
    m_acceleration(0.f, 0.f),
    m_velocity(0.f, 0.f),
    m_maxSpeed(std::numeric_limits<float>::max()),
    m_slowdownDistance(0.f),
    m_targetPosition(0.f, 0.f),
    m_targeted(false)
    {
    }

void fe::camera::setSize(const fe::Vector2d size)
    {
        setSize(size.x, size.y);
    }

void fe::camera::setSize(float x, float y)
    {
        m_size.x = x;
        m_size.y = y;
    }

void fe::camera::setPosition(const fe::Vector2d position)
    {
        setPosition(position.x, position.y);
    }

void fe::camera::setPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
    }

void fe::camera::setVelocity(const fe::Vector2d velocity)
    {
        setVelocity(velocity.x, velocity.y);
    }

void fe::camera::setVelocity(float x, float y)
    {
        m_velocity.x = x;
        m_velocity.y = y;
    }

void fe::camera::setAcceleration(const fe::Vector2d acceleration)
    {
        setAcceleration(acceleration.x, acceleration.y);
    }

void fe::camera::setAcceleration(float x, float y)
    {
        m_acceleration.x = x;
        m_acceleration.y = y;
    }

void fe::camera::setMaxSpeed(float speed)
    {
        m_maxSpeed = speed;
    }

void fe::camera::setSlowdownDistance(float distance)
    {
        m_slowdownDistance = distance;
    }

void fe::camera::moveToPosition(const fe::Vector2d position)
    {
       moveToPosition(position.x, position.y);
    }

void fe::camera::moveToPosition(float x, float y)
    {
        m_targetPosition.x = x;
        m_targetPosition.y = y;
        m_targeted = true;
    }

void fe::camera::move(const fe::Vector2d position)
    {
        move(position.x, position.y);
    }

void fe::camera::move(float x, float y)
    {
        setPosition(m_position.x + x, m_position.y + y);
    }

fe::Vector2d fe::camera::getSize() const
    {
        return m_size;
    }

fe::Vector2d fe::camera::getPosition() const
    {
        return m_position;
    }

fe::Vector2d fe::camera::getAcceleration() const
    {
        return m_acceleration;
    }

fe::Vector2d fe::camera::getVelocity() const
    {
        return m_velocity;
    }

float fe::camera::getMaxSpeed() const
    {
        return m_maxSpeed;
    }

float fe::camera::getSlowdownDistance() const
    {
        return m_slowdownDistance;
    }

void fe::camera::updateCamera(float dt)
    {
        if (m_targeted)
            {
                fe::Vector2d targetLine = m_targetPosition - m_position;
                float distance = std::sqrt(targetLine.x * targetLine.x + targetLine.y * targetLine.y);
                if (distance > 1.f) // if its less than 1 pixel away the camera is at the position. Since we round to an integer there wont be any stutter
                    {
                        float possibleSpeed = m_maxSpeed * (distance / m_slowdownDistance);
                        float speed = std::min(possibleSpeed, m_maxSpeed);

                        fe::Vector2d desiredVel = targetLine * (speed / distance);
                        fe::Vector2d steeringForce = desiredVel - m_velocity;

                        m_acceleration += steeringForce;
                    }
                else
                    {
                        m_targeted = false;
                        m_velocity = fe::Vector2d();
                        setPosition(m_targetPosition);
                    }
            }

        m_velocity += m_acceleration;
        m_velocity.clamp(m_maxSpeed);
        move(m_velocity * dt);

        fe::Vector2<int> cameraPos = m_position;
        fe::Vector2<int> cameraSize = m_size;

        m_view.setCenter({ (float)cameraPos.x, (float)cameraPos.y });
        m_view.setSize({ (float)cameraSize.x, (float)cameraSize.y });

        m_acceleration = fe::Vector2d();
    }

const sf::View &fe::camera::getView()
    {
        return m_view;
    }

fe::camera &fe::camera::operator=(const fe::camera &rhs)
    {
        if (&rhs != this)
            {
                m_view = rhs.m_view;
                m_position = rhs.m_position;
                m_size = rhs.m_size;
                m_acceleration = rhs.m_acceleration;
                m_velocity = rhs.m_velocity;
                m_maxSpeed = rhs.m_maxSpeed;
                m_slowdownDistance = rhs.m_slowdownDistance;
                m_targetPosition = rhs.m_targetPosition;
                m_targeted = rhs.m_targeted;
            }

        return *this;
    }
