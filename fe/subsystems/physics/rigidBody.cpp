#include "rigidBody.hpp"
#include "../serializer/serializerID.hpp"

void fe::rigidBody::serialize(fe::serializerID &serializer) const
    {
        serializer.write("positionX", m_positionX);
        serializer.write("positionY", m_positionY);
        serializer.write("oldPosX", m_oldPositionX);
        serializer.write("oldPosY", m_oldPositionY);
        serializer.write("velocityX", m_velocityX);
        serializer.write("velocityY", m_velocityY);
        serializer.write("forceX", m_forceX);
        serializer.write("forceY", m_forceY);
        serializer.write("impulseX", m_impulseX);
        serializer.write("impulseY", m_impulseY);
        serializer.write("maxSpeed", m_maxSpeed);
        serializer.write("mass", m_mass);
        serializer.write("friction", m_frictionCoeff);
        serializer.write("enabled", m_enabled);
    }

void fe::rigidBody::deserialize(fe::serializerID &serializer)
    {
        m_positionX = serializer.read<float>("positionX");
        m_positionY = serializer.read<float>("positionY");
        m_oldPositionX = serializer.read<float>("oldPosX");
        m_oldPositionY = serializer.read<float>("oldPosY");
        m_velocityX = serializer.read<float>("velocityX");
        m_velocityY = serializer.read<float>("velocityY");
        m_forceX = serializer.read<float>("forceX");
        m_forceY = serializer.read<float>("forceY");
        m_impulseX = serializer.read<float>("impulseX");
        m_impulseY = serializer.read<float>("impulseY");
        m_maxSpeed = serializer.read<float>("maxSpeed");
        m_mass = serializer.read<float>("mass");
        m_frictionCoeff = serializer.read<float>("friction");
        m_enabled = serializer.read<bool>("enabled");
    }

float fe::rigidBody::getVelocityX() const
    {
        return m_velocityX;
    }

float fe::rigidBody::getVelocityY() const
    {
        return m_velocityY;
    }

float fe::rigidBody::getForceX() const
    {
        return m_forceX;
    }

float fe::rigidBody::getForceY() const
    {
        return m_forceY;
    }

float fe::rigidBody::getImpulseX() const
    {
        return m_impulseX;
    }

float fe::rigidBody::getImpulseY() const
    {
        return m_impulseY;
    }

float fe::rigidBody::getPositionX() const
    {
        return m_positionX;
    }

float fe::rigidBody::getPositionY() const
    {
        return m_positionY;
    }

float fe::rigidBody::getNormalForceX() const
    {
        return m_normalForceX;
    }

float fe::rigidBody::getNormalForceY() const
    {
        return m_normalForceY;
    }

float fe::rigidBody::getSpeed() const
    {
        return fe::Vector2d(m_velocityX, m_velocityY).magnitude();
    }

float fe::rigidBody::getTotalForce() const
    {
        return fe::Vector2d(getForceX(), getForceY()).magnitude();
    }

float fe::rigidBody::getHeading() const
    {
        return fe::Vector2d(m_velocityX, m_velocityY).normalize().magnitude();
    }

float fe::rigidBody::getMass() const
    {
        return m_mass;
    }

float fe::rigidBody::getMaxVelocity() const
    {
        return m_maxSpeed;
    }

float fe::rigidBody::getFrictionCoefficient() const
    {
        return m_frictionCoeff;
    }

fe::Vector2d fe::rigidBody::getVelocity() const
    {
        return { m_velocityX, m_velocityY };
    }

fe::Vector2d fe::rigidBody::getForce() const
    {
        return { getForceX(), getForceY() };
    }

fe::Vector2d fe::rigidBody::getDirection() const
    {
        return fe::Vector2d(m_velocityX, m_velocityY).normalize();
    }

fe::Vector2d fe::rigidBody::getPosition() const
    {
        return fe::Vector2d(m_positionX, m_positionY);
    }

void fe::rigidBody::setMass(float newMass)
    {
        m_mass = newMass;
    }

void fe::rigidBody::setMaxSpeed(float maxSpeed)
    {
        m_maxSpeed = maxSpeed;
    }

void fe::rigidBody::setFrictionCoefficient(float fricCoeff)
    {
        m_frictionCoeff = fricCoeff;
    }

void fe::rigidBody::setNormalForce(float x, float y)
    {
        m_normalForceX = x;
        m_normalForceY = y;
    }

void fe::rigidBody::applyForce(float x, float y)
    {
        if (!m_enabled) return;
        m_impulseX += x;
        m_impulseY += y;
    }

void fe::rigidBody::applyForce(fe::lightVector2d force)
    {
        m_impulseX += force.x;
        m_impulseY += force.y;
    }

void fe::rigidBody::setVelocity(float x, float y)
    {
        m_velocityX = x;
        m_velocityY = y;
    }

void fe::rigidBody::setVelocity(fe::lightVector2d velocity)
    {
        m_velocityX += velocity.x;
        m_velocityY += velocity.y;
    }

void fe::rigidBody::setForce(float x, float y)
    {
        m_forceX = x;
        m_forceY = y;
    }

void fe::rigidBody::setForce(fe::lightVector2d force)
    {
        m_forceX = force.x;
        m_forceY = force.y;
    }

void fe::rigidBody::setDirection(float x, float y)
    {
        if (!m_enabled) return;

        m_velocityX = getSpeed() * fe::Vector2d(x, y).normalize().x;
        m_velocityY = getSpeed() * fe::Vector2d(x, y).normalize().y;
    }

void fe::rigidBody::setDirection(fe::lightVector2d direction)
    {
        if (!m_enabled) return;

        m_velocityX = getSpeed() * fe::Vector2d(direction.x, direction.y).normalize().x;
        m_velocityY = getSpeed() * fe::Vector2d(direction.x, direction.y).normalize().y;
    }

void fe::rigidBody::setPosition(float x, float y)
    {
        m_positionX = x;
        m_positionY = y;
    }

void fe::rigidBody::setPosition(fe::lightVector2d position)
    {
        m_positionX = position.x;
        m_positionY = position.y;
    }

fe::rigidBody::rigidBody() : 
    m_mass(1.f),
    m_maxSpeed(0.f),
    m_frictionCoeff(1.f),
    m_enabled(true),
    m_positionX(0.f),
    m_positionY(0.f),
    m_velocityX(0.f),
    m_velocityY(0.f),
    m_forceX(0.f),
    m_forceY(0.f),
    m_impulseX(0.f),
    m_impulseY(0.f) ,
    m_normalForceX(0.f),
    m_normalForceY(0.f)
    {
    }

fe::rigidBody::rigidBody(float mass) : 
    m_mass(mass),
    m_maxSpeed(0.f),
    m_frictionCoeff(1.f),
    m_enabled(true),
    m_positionX(0.f),
    m_positionY(0.f),
    m_velocityX(0.f),
    m_velocityY(0.f),
    m_forceX(0.f),
    m_forceY(0.f),
    m_impulseX(0.f),
    m_impulseY(0.f),
    m_normalForceX(0.f),
    m_normalForceY(0.f)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed) : 
    m_mass(mass),
    m_maxSpeed(maxSpeed),
    m_frictionCoeff(1.f),
    m_enabled(true),
    m_positionX(0.f),
    m_positionY(0.f),
    m_velocityX(0.f),
    m_velocityY(0.f),
    m_forceX(0.f),
    m_forceY(0.f),
    m_impulseX(0.f),
    m_impulseY(0.f),
    m_normalForceX(0.f),
    m_normalForceY(0.f)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed, float frictionCoeff) : 
    m_mass(mass),
    m_maxSpeed(maxSpeed),
    m_frictionCoeff(frictionCoeff),
    m_enabled(true),
    m_positionX(0.f),
    m_positionY(0.f),
    m_velocityX(0.f),
    m_velocityY(0.f),
    m_forceX(0.f),
    m_forceY(0.f),
    m_impulseX(0.f),
    m_impulseY(0.f),
    m_normalForceX(0.f),
    m_normalForceY(0.f)
    {
    }

void fe::rigidBody::enable(bool value)
    {
        m_enabled = value;
    }

bool fe::rigidBody::getEnabled() const
    {
        return m_enabled;
    }

void fe::rigidBody::setMetaData(void *data)
    {
        m_metaData = data;
    }

void *fe::rigidBody::getMetaData() const
    {
        return m_metaData;
    }
