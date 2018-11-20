#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/physics/rigidBody.hpp"
#include "fe/subsystems/graphic/renderObject/renderObject.hpp"
#include "fe/subsystems/gameState/gameState.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/graphic/renderObject/sceneGraph.hpp"
#include "fe/subsystems/physics/physicsEngine.hpp"
#include "fe/subsystems/collision/collisionWorld.hpp"
#include "fe/subsystems/serializer/serializerID.hpp"
#include "fe/subsystems/entitySpawner/userEntityObject.hpp"

fe::baseEntity::baseEntity(fe::entityModules modules, fe::userEntityObject *scriptObject, bool staticObject) :
    m_killEntity(false),
    m_enabled(false),
    m_renderObject(nullptr),
    m_rigidBody(nullptr),
    m_collisionBody(nullptr),
    m_animationActor(nullptr),
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
    scriptObject(this),
    m_baseEntityUtility(this)
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
        //enable(false);
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

        if (m_enabledModulesEnum & entityModules::ANIMATION)
            {
                world.getEntityWorld().getAnimator().free(m_animationActor);
                m_animationActor = nullptr;
            }

        if (m_entityScriptObject) { m_entityScriptObject->shutDown(); }
    }

void fe::baseEntity::onAdd(fe::gameWorld &world)
    {
        if (m_entityScriptObject) { m_entityScriptObject->onAdd(this, world); }
    }

void fe::baseEntity::onRemove(fe::gameWorld &world)
    {
        if (m_entityScriptObject) { m_entityScriptObject->onRemove(this, world); }
    }

void fe::baseEntity::enable(bool value)
    {
        m_enabled = value;

#if _DEBUG
        if (!m_enabled)
            {
                int i = 0;
            }
#endif
        
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

        if (m_enabledModulesEnum & entityModules::ANIMATION)
            {
                m_animationActor->play(value);
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
        int i = 0;
    }

void fe::baseEntity::update()
    {
        m_baseEntityUtility.update();
        if (m_entityScriptObject) { m_entityScriptObject->update(this); }
    }

void fe::baseEntity::fixedUpdate(float deltaTime)
    {
        if (m_entityScriptObject) { m_entityScriptObject->fixedUpdate(this, deltaTime);}
    }

void fe::baseEntity::postUpdate()
    {
        if (m_entityScriptObject) { m_entityScriptObject->postUpdate(this); }
    }

void fe::baseEntity::updateModules()
    {
        if (m_rigidBody)
            {
                float posX = m_rigidBody->getPositionX();
                float posY = m_rigidBody->getPositionY();

                m_positionX = posX;
                m_positionY = posY;

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
                        m_collisionBody->m_aabb.m_globalPositionX = m_renderObject->m_transform.getPosition().x;
                        m_collisionBody->m_aabb.m_globalPositionY = m_renderObject->m_transform.getPosition().y;
                        m_collisionBody->m_moved = true;
                    }

                if (!m_rigidBody)
                    {
                        m_positionX = m_renderObject->m_transform.getPosition().x;
                        m_positionY = m_renderObject->m_transform.getPosition().y;
                    }
            }

        if (m_collisionBody)
            {
                m_collisionBody->m_aabb.m_globalPositionX += m_collisionBody->m_aabb.m_offsetX;
                m_collisionBody->m_aabb.m_globalPositionY += m_collisionBody->m_aabb.m_offsetY;
            }
    }

void fe::baseEntity::setScriptObject(fe::userEntityObject *obj)
    {
        m_entityScriptObject = obj;
    }

fe::userEntityObject *fe::baseEntity::getEntityObject() const
    {
        return m_entityScriptObject;
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

void fe::baseEntity::setRotation(float degree)
    {
        if (m_renderObject)
            {
                m_renderObject->m_transform.setRotation(degree * (3.14159f / 180.f));
            }

        m_rotation = degree;
    }

float fe::baseEntity::getRotation() const
    {
        return m_rotation;
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

fe::animationActor *fe::baseEntity::getActor() const
    {
        return m_animationActor;
    }

fe::entityModules fe::baseEntity::getModules() const
    {
        return m_enabledModulesEnum;
    }

fe::baseEntityUtilities &fe::baseEntity::getUtilities()
    {
        return m_baseEntityUtility;
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

        if (m_animationActor)
            {
                FE_LOG_WARNING("fe::baseEntity::m_animationActor already allocated. Possible memory leak");
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
                m_rigidBody->setMetaData(this);
            }

        if (m_enabledModulesEnum & fe::entityModules::COLLISION_BODY)
            {
                m_collisionBody = fe::engine::get().getCollisionWorld().createCollider(0.f, 0.f);
                m_collisionBody->m_owner = this;
            }

        if (m_enabledModulesEnum & fe::entityModules::ANIMATION)
            {
#if _DEBUG
                if (static_cast<fe::renderObject*>(m_renderObject)->m_debugHeader != 0xDEAD)
                    {
                        FE_LOG_ERROR("Entities current render type is TEXT and ANIMATION requires RENDER_OBJECT type");
                    }
#endif
                m_animationActor = currentState.getGameWorld().getEntityWorld().getAnimator().alloc(static_cast<fe::renderObject*>(m_renderObject));
            }
    }

void fe::baseEntity::addComponent(fe::Handle handle)
    {
        m_components.emplace_back(handle, true);
    }

void fe::baseEntity::removeComponent(fe::Handle handle)
    {
        m_components.erase(std::find_if(m_components.begin(), m_components.end(), [handle](std::pair<fe::Handle, bool> value) { return value.first == handle; }), m_components.end());
    }

void fe::baseEntity::enableComponent(fe::Handle handle, bool value)
    {
        auto it = std::find_if(m_components.begin(), m_components.end(), [handle](std::pair<fe::Handle, bool> value) { return value.first == handle; });
        if (it != m_components.end())
            {
                it->second = value;
            }
    }

const std::vector<std::pair<fe::Handle, bool>> &fe::baseEntity::getAllComponents() const
    {
        return m_components;
    }

bool fe::baseEntity::isComponentEnabled(fe::Handle handle) const
    {
        bool enabled = false;
        auto it = std::find_if(m_components.begin(), m_components.end(), [handle](std::pair<fe::Handle, bool> value) { return value.first == handle; });
        if (it != m_components.end())
            {
                enabled = it->second;
            }
        return enabled;
    }
