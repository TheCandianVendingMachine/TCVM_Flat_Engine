#include "baseEntity.hpp"
#include "../subsystems/physics/rigidBody.hpp"
#include "../subsystems/graphic/renderObject/renderObject.hpp"
#include "../subsystems/gameState/gameState.hpp"
#include "../engine.hpp"
#include "../subsystems/graphic/renderObject/sceneGraph.hpp"
#include "../subsystems/physics/physicsEngine.hpp"
#include "../subsystems/physics/collision/collisionWorld.hpp"
#include "../subsystems/serializer/serializerID.hpp"
#include "../subsystems/entitySpawner/userEntityObject.hpp"

fe::baseEntity::baseEntity(fe::entityModules modules, fe::userEntityObject *scriptObject, bool staticObject) :
    m_killEntity(false),
    m_enabled(false),
    m_renderObject(nullptr),
    m_rigidBody(nullptr),
    m_collisionBody(nullptr),
    m_moved(false),
    m_allocatedModules(false),
    m_static(staticObject),
    m_positionX(0.f),
    m_positionY(0.f),
    m_sizeX(0.f),
    m_sizeY(0.f),
    m_colour(sf::Color::White),
    m_enabledModulesEnum(modules),
    m_entityScriptObject(scriptObject),
    scriptObject(this)
{}

void fe::baseEntity::initialize(fe::gameWorld &world, int connected, const fe::fontData &font)
    {
        if (m_renderObject)
            {
                world.getSceneGraph().createSceneGraphObject(m_renderObject, connected, font);
                m_renderObject->m_static = m_static;
            }

        if (m_rigidBody && !m_static)
            {
                        
            }

        if (m_collisionBody)
            {
                m_collisionBody->m_static = m_static;
            }

        setPosition(m_positionX, m_positionY);
        setSize(m_sizeX, m_sizeY);
        setColour(m_colour);

        enable(true);
    }

void fe::baseEntity::deinitialize(fe::gameWorld &world)
    {
        enable(false);
        if (m_enabledModulesEnum & entityModules::RENDER_OBJECT || m_enabledModulesEnum & entityModules::RENDER_TEXT)
            {
                world.getSceneGraph().deleteSceneObject(m_renderObject);
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

void fe::baseEntity::onAdd(fe::gameWorld &world)
    {
        m_entityScriptObject->onAdd(this, world);
    }

void fe::baseEntity::onRemove(fe::gameWorld &world)
    {
        m_entityScriptObject->onRemove(this, world);
    }

void fe::baseEntity::enable(bool value)
    {
        m_enabled = value;
        
        enableDrawing(value);
        enablePhysics(value);
        enableCollision(value);
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

void fe::baseEntity::enableDrawing(bool value)
    {
        if (m_enabledModulesEnum & entityModules::RENDER_OBJECT || m_enabledModulesEnum & entityModules::RENDER_TEXT)
            {
                m_renderObject->m_draw = value;
            }
    }

void fe::baseEntity::enablePhysics(bool value)
    {
        if (m_enabledModulesEnum & entityModules::RIGID_BODY)
            {
                m_rigidBody->enable(value);
            }
    }

void fe::baseEntity::enableCollision(bool value)
    {
        if (m_enabledModulesEnum & entityModules::COLLISION_BODY)
            {
                m_collisionBody->m_enabled = value;
            }
    }

void fe::baseEntity::onDestroy(fe::baseGameState &state)
    {
        
    }

void fe::baseEntity::update()
    {
        m_entityScriptObject->update(this);
    }

void fe::baseEntity::postUpdate()
    {
        m_entityScriptObject->postUpdate(this);
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

                m_moved = abs(m_rigidBody->m_oldPositionX - m_rigidBody->m_positionX) > 0.0001f || abs(m_rigidBody->m_oldPositionY - m_rigidBody->m_positionY) > 0.0001f;

                if (m_collisionBody) 
                    {
                        m_collisionBody->m_aabb.m_globalPositionX = posX;
                        m_collisionBody->m_aabb.m_globalPositionY = posY;
                        m_collisionBody->m_moved = m_moved;
                    }
            }

        if (m_renderObject)
            {
                if (m_collisionBody)
                    {
                        m_collisionBody->m_aabb.m_globalPositionX = m_renderObject->m_tempTransform.getPosition().x;
                        m_collisionBody->m_aabb.m_globalPositionY = m_renderObject->m_tempTransform.getPosition().y;
                        m_collisionBody->m_moved = true;
                    }
            }
    }

void fe::baseEntity::setScriptObject(fe::userEntityObject *obj)
    {
        m_entityScriptObject = obj;
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
                m_collisionBody->m_aabb.m_globalPositionX = x;
                m_collisionBody->m_aabb.m_globalPositionY = y;
                m_collisionBody->m_moved = true;
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

void fe::baseEntity::setName(const std::string &name)
    {
        m_name = name;
    }

std::string fe::baseEntity::getName() const
    {
        return m_name;
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

fe::entityModules fe::baseEntity::getModules() const
    {
        return m_enabledModulesEnum;
    }

void fe::baseEntity::createModules(fe::baseGameState &currentState)
    {
        if (m_allocatedModules)
            {
                FE_LOG_WARNING("fe::baseEntity has already allocated its modules. Cannot allocate more");
                return;
            }

        m_allocatedModules = true;
        if (m_renderObject)
            {
                FE_LOG_WARNING("fe::baseEntity::m_renderObject already allocated. Possible Memory Leak");
            }

        if (m_rigidBody)
            {
                FE_LOG_WARNING("fe::baseEntity::m_rigidBody already allocated. Possible memory leak");
            }

        if (m_collisionBody)
            {
                FE_LOG_WARNING("fe::baseEntity::m_collisionBody already allocated. Possible memory leak");
            }

        if (m_enabledModulesEnum & fe::entityModules::RENDER_OBJECT)
            {
                m_renderObject = currentState.getGameWorld().getSceneGraph().allocateRenderObject();
            }
        else if (m_enabledModulesEnum & fe::entityModules::RENDER_TEXT)
            {
                m_renderObject = currentState.getGameWorld().getSceneGraph().allocateRenderText();
            }

        if (m_enabledModulesEnum & fe::entityModules::RIGID_BODY)
            {
                m_rigidBody = fe::engine::get().getPhysicsEngine().createRigidBody();
            }

        if (m_enabledModulesEnum & fe::entityModules::COLLISION_BODY)
            {
                m_collisionBody = fe::engine::get().getCollisionWorld().createCollider(0.f, 0.f);
            }
    }
