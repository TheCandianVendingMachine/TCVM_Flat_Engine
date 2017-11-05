#include "baseEntity.hpp"
#include "../subsystems/physics/rigidBody.hpp"
#include "../subsystems/graphic/renderObject/renderObject.hpp"
#include "../engine.hpp"
#include "../subsystems/graphic/renderObject/sceneGraph.hpp"
#include "../subsystems/physics/physicsEngine.hpp"
#include "../subsystems/physics/collision/collisionWorld.hpp"

fe::baseEntity::baseEntity(fe::entityModules modules, bool staticObject) :
    m_killEntity(false),
    m_enabled(false),
    m_renderObject(nullptr),
    m_rigidBody(nullptr),
    m_collisionBody(nullptr),
    m_moved(false),
    m_static(staticObject),
    m_positionX(0.f),
    m_positionY(0.f),
    m_sizeX(0.f),
    m_sizeY(0.f),
    m_colour(sf::Color::White),
    m_enabledModulesEnum(modules)
{}

void fe::baseEntity::deinitialize()
    {
        enable(false);
        if (m_enabledModulesEnum & entityModules::RENDER_OBJECT || m_enabledModulesEnum & entityModules::RENDER_TEXT)
            {
                fe::engine::get().getStateMachine().getSceneGraph().deleteSceneObject(m_renderObject);
                m_renderObject = nullptr;
            }

        if (m_enabledModulesEnum & entityModules::RIGID_BODY)
            {
                fe::engine::get().getPhysicsEngine().deleteRigidBody(m_rigidBody);
                m_rigidBody = nullptr;
            }

        if (m_enabledModulesEnum & entityModules::COLLISION_BODY)
            {
                fe::engine::get().getCollisionWorld().deleteCollider(m_collisionBody);
                m_collisionBody = nullptr;
            }
    }

void fe::baseEntity::enable(bool value)
    {
        m_enabled = value;
        if (m_enabledModulesEnum & entityModules::RENDER_OBJECT || m_enabledModulesEnum & entityModules::RENDER_TEXT)
            {
                m_renderObject->m_draw = value;
            }

        if (m_enabledModulesEnum & entityModules::RIGID_BODY)
            {
                m_rigidBody->enable(value);
            }

        if (m_enabledModulesEnum & entityModules::COLLISION_BODY)
            {
                m_collisionBody->m_enabled = value;
            }
    }

bool fe::baseEntity::getEnabled() const
    {
        return m_enabled;
    }

void fe::baseEntity::kill(bool value)
    {
        m_killEntity = value;
    }

bool fe::baseEntity::isKilled() const
    {
        return m_killEntity;
    }

void fe::baseEntity::setHandle(fe::Handle handle)
    {
        m_handle = handle;
    }

fe::Handle fe::baseEntity::getHandle() const
    {
        return m_handle;
    }

void fe::baseEntity::enableCollision(bool value)
    {
        m_collisionBody->m_enabled = value;
    }

void fe::baseEntity::onDestroy(fe::baseGameState &state)
    {
        
    }

void fe::baseEntity::updateModules()
    {
        if (m_rigidBody)
            {
                float posX = m_rigidBody->getPositionX();
                float posY = m_rigidBody->getPositionY();

                if (m_renderObject) 
                    {
                        m_renderObject->m_transform.setPosition(posX, posY);
                    }

                if (m_collisionBody) 
                    {
                        m_collisionBody->m_aabb.m_positionX = posX;
                        m_collisionBody->m_aabb.m_positionY = posY;
                    }

                m_moved = abs(m_rigidBody->m_oldPositionX - m_rigidBody->m_positionX) > 0.0001f || abs(m_rigidBody->m_oldPositionY - m_rigidBody->m_positionY) > 0.0001f;
            }

        if (m_renderObject)
            {
                if (m_collisionBody)
                    {
                        m_collisionBody->m_aabb.m_globalPositionX = m_renderObject->m_tempTransform.getPosition().x;
                        m_collisionBody->m_aabb.m_globalPositionY = m_renderObject->m_tempTransform.getPosition().y;
                    }
            }
    }

void fe::baseEntity::setPosition(float x, float y)
    {
        m_positionX = x;
        m_positionY = y;

        if (m_renderObject)
            {
                m_renderObject->m_transform.setPosition(x, y);
            }

        if (m_rigidBody)
            {
                m_rigidBody->setPosition(x, y);
            }

        if (m_collisionBody)
            {
                m_collisionBody->m_aabb.m_positionX = x;
                m_collisionBody->m_aabb.m_positionY = y;
            }
    }

void fe::baseEntity::setPosition(fe::Vector2d position)
    {
        setPosition(position.x, position.y);
    }

void fe::baseEntity::setPosition(fe::lightVector2d position)
    {
        setPosition(position.x, position.y);
    }

void fe::baseEntity::setSize(float x, float y)
    {
        if (m_renderObject)
            {
                m_renderObject->setSize(x, y);
            }

        if (m_collisionBody)
            {
                m_collisionBody->m_aabb.m_sizeX = x;
                m_collisionBody->m_aabb.m_sizeY = y;
            }

        m_sizeX = x;
        m_sizeY = y;
    }

void fe::baseEntity::setSize(fe::Vector2d size)
    {
        setSize(size.x, size.y);
    }

void fe::baseEntity::setSize(fe::lightVector2d size)
    {
        setSize(size.x, size.y);
    }

void fe::baseEntity::setColour(const sf::Color colour)
    {
        m_colour = colour;
        if (m_renderObject)
            {
                m_renderObject->m_vertColour[0] = colour.r;
                m_renderObject->m_vertColour[1] = colour.g;
                m_renderObject->m_vertColour[2] = colour.b;
                m_renderObject->m_vertColour[3] = colour.a;
            }
    }

fe::lightVector2d fe::baseEntity::getPosition() const
    {
        return fe::lightVector2d(m_positionX, m_positionY);
    }

fe::lightVector2d fe::baseEntity::getSize() const
    {
        return fe::lightVector2d(m_sizeX, m_sizeY);
    }

sf::Color fe::baseEntity::getColour() const
    {
        return m_colour;
    }

fe::sceneGraphObject *fe::baseEntity::getRenderObject() const
    {
        return m_renderObject;
    }

fe::rigidBody *fe::baseEntity::getRigidBody() const
    {
        return m_rigidBody;
    }

fe::collider *fe::baseEntity::getCollider() const
    {
        return m_collisionBody;
    }

fe::Handle fe::baseEntity::GUID() const
    {
        return m_handle;
    }

