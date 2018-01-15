// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../typeDefines.hpp"
#include "../engine.hpp"
#include "../subsystems/physics/rigidBody.hpp"
#include "../subsystems/gameState/gameStateMachine.hpp"
#include "../subsystems/gameState/gameWorld.hpp"
#include "../subsystems/physics/collision/collisionBody.hpp"
#include "../subsystems/physics/collision/collisionWorld.hpp"
#include "../subsystems/graphic/renderObject/sceneGraph.hpp"
#include "../subsystems/physics/physicsEngine.hpp"
#include "../objectManagement/guid.hpp"
#include "../subsystems/serializer/serializerID.hpp"
#include <SFML/Graphics/Color.hpp>

namespace fe
    {
        struct sceneGraphObject;
        class rigidBody;
        class baseGameState;
        class gameWorld;

        enum class entityModules : std::int16_t
            {
                NONE            = 0,
                RENDER_OBJECT   = 1 << 0,
                RENDER_TEXT     = 1 << 1,
                RIGID_BODY      = 1 << 2,
                COLLISION_BODY  = 1 << 3,
            };

        inline constexpr entityModules operator | (entityModules lhs, entityModules rhs)
            {
                return static_cast<entityModules>(static_cast<std::int16_t>(lhs) | static_cast<std::int16_t>(rhs));
            }

        inline constexpr int16_t operator & (entityModules lhs, entityModules rhs)
            {
                return static_cast<std::int16_t>(lhs) & static_cast<std::int16_t>(rhs);
            }

        class baseEntity : public fe::guid
            {
                protected:
                    fe::sceneGraphObject *m_renderObject;
                    fe::rigidBody *m_rigidBody;
                    fe::collider *m_collisionBody;

                    fe::entityModules m_enabledModulesEnum;
                    std::int16_t m_enabledModulesNum;

                    fe::Handle m_handle;

                    // Local variables to keep updated for debugging. Is only used to set position on initialization
                    float m_positionX;
                    float m_positionY;
                    float m_sizeX;
                    float m_sizeY;
                    sf::Color m_colour;

                    bool m_killEntity;
                    bool m_enabled;
                    bool m_moved;
                    bool m_static;

                    friend class gameWorld;

                    SERIALIZE_ID("defaultVars",
                        "modules", m_enabledModulesNum,
                        "handle", m_handle,
                        "posX", m_positionX,
                        "posY", m_positionY,
                        "sizeX", m_sizeX,
                        "sizeY", m_sizeY,
                        "enabled", m_enabled,
                        "moved", m_moved,
                        "static", m_static);

                public:
                    FLAT_ENGINE_API baseEntity(fe::entityModules modules, bool staticObject);
                    template<typename ...Args>
                    void initialize(fe::gameWorld &world, Args &&...args);
                    FLAT_ENGINE_API void deinitialize(fe::gameWorld &world);

                    FLAT_ENGINE_API void enable(bool value);
                    FLAT_ENGINE_API bool getEnabled() const;

                    FLAT_ENGINE_API void kill(bool value);
                    FLAT_ENGINE_API bool isKilled() const;

                    FLAT_ENGINE_API void setHandle(fe::Handle handle);
                    FLAT_ENGINE_API fe::Handle getHandle() const;

                    FLAT_ENGINE_API void enableCollision(bool value);

                    FLAT_ENGINE_API void onDestroy(fe::baseGameState &state);

                    FLAT_ENGINE_API virtual void onAdd(fe::baseGameState &state) {}
                    FLAT_ENGINE_API virtual void onRemove(fe::baseGameState &state) {}

                    FLAT_ENGINE_API virtual void update() {}
                    FLAT_ENGINE_API virtual void postUpdate() {}
                    FLAT_ENGINE_API void updateModules();

                    FLAT_ENGINE_API void setPosition(float x, float y);
                    FLAT_ENGINE_API void setPosition(fe::Vector2d position);
                    FLAT_ENGINE_API void setPosition(fe::lightVector2d position);

                    FLAT_ENGINE_API void setSize(float x, float y);
                    FLAT_ENGINE_API void setSize(fe::Vector2d size);
                    FLAT_ENGINE_API void setSize(fe::lightVector2d size);

                    FLAT_ENGINE_API void setColour(const sf::Color colour);

                    FLAT_ENGINE_API fe::lightVector2d getPosition() const;
                    FLAT_ENGINE_API fe::lightVector2d getSize() const;
                    FLAT_ENGINE_API sf::Color getColour() const;

                    FLAT_ENGINE_API fe::sceneGraphObject *getRenderObject() const;
                    FLAT_ENGINE_API fe::rigidBody *getRigidBody() const;
                    FLAT_ENGINE_API fe::collider *getCollider() const;

            };

        template<typename ...Args>
        void fe::baseEntity::initialize(fe::gameWorld &world, Args &&...args)
            {
                if (m_enabledModulesEnum & fe::entityModules::RENDER_OBJECT)
                    {
                        m_renderObject = world.getSceneGraph().allocateRenderObject();
                    }
                else if (m_enabledModulesEnum & fe::entityModules::RENDER_TEXT)
                    {
                        m_renderObject = world.getSceneGraph().allocateRenderText();
                    }

                if (m_enabledModulesEnum & fe::entityModules::RIGID_BODY)
                    {
                        m_rigidBody = fe::engine::get().getPhysicsEngine().createRigidBody();
                    }

                if (m_enabledModulesEnum & fe::entityModules::COLLISION_BODY)
                    {
                        m_collisionBody = fe::engine::get().getCollisionWorld().createCollider(0.f, 0.f);
                    }

                if (m_renderObject)
                    {
                        world.getSceneGraph().createSceneGraphObject(m_renderObject, std::forward<Args>(args)...);
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
    }