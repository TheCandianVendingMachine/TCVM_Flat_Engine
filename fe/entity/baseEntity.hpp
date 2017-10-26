// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../typeDefines.hpp"
#include "../subsystems/physics/rigidBody.hpp"
#include "../subsystems/physics/collision/collisionBody.hpp"
#include "../subsystems/graphic/renderObject/sceneGraph.hpp"
#include "../subsystems/physics/physicsEngine.hpp"
#include "../subsystems/physics/collision/collisionWorld.hpp"
#include <SFML/Graphics/Color.hpp>

namespace fe
    {
        struct sceneGraphObject;
        class rigidBody;
        class baseGameState;

        enum class entityModules : std::int16_t
            {
                RENDER_OBJECT   = 1 << 0,
                RENDER_TEXT     = 1 << 1,
                RIGID_BODY      = 1 << 2,
                COLLISION_BODY  = 1 << 3,
            };

        inline entityModules operator | (entityModules lhs, entityModules rhs)
            {
                return static_cast<entityModules>(static_cast<std::int16_t>(lhs) | static_cast<std::int16_t>(rhs));
            }

        inline int16_t operator & (entityModules lhs, entityModules rhs)
            {
                return static_cast<std::int16_t>(lhs) & static_cast<std::int16_t>(rhs);
            }

        class baseEntity
            {
                protected:
                    fe::sceneGraphObject *m_renderObject;
                    fe::rigidBody *m_rigidBody;
                    fe::collider *m_collisionBody;

                    fe::Handle m_handle;

                    entityModules m_modulesEnabled;

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

                    template<typename T = void>
                    void initRenderObject();
                    template<typename T0, typename ...Args, typename std::enable_if<std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type = 0>
                    void initRenderObject(T0 &&arg0 = T0(), Args &&...args);
                    template<typename T0, typename ...Args, typename std::enable_if<!std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type = 0>
                    void initRenderObject(T0 &&arg0 = T0(), Args &&...args);

                    template<typename T = void>
                    void initRenderText();
                    template<typename T0, typename ...Args, typename std::enable_if<!std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type = 0>
                    void initRenderText(T0 &&arg0 = T0(), Args &&...args);
                    template<typename T0, typename ...Args, typename std::enable_if<std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type = 0>
                    void initRenderText(T0 &&arg0 = T0(), Args &&...args);

                public:
                    FLAT_ENGINE_API baseEntity(entityModules modules, bool staticObject);
                    template<typename ...Args>
                    FLAT_ENGINE_API void initialize(Args &&...args);
                    FLAT_ENGINE_API void deinitialize();

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

                    FLAT_ENGINE_API fe::Handle GUID() const;

            };

        template<typename ...Args>
        void fe::baseEntity::initialize(Args &&...args)
            {
                if (m_modulesEnabled & entityModules::RENDER_OBJECT)
                    {
                        initRenderObject(std::forward<Args>(args)...);
                        m_renderObject->m_static = m_static;
                    }
                else if (m_modulesEnabled & entityModules::RENDER_TEXT)
                    {
                        initRenderText(std::forward<Args>(args)...);
                        m_renderObject->m_static = m_static;
                    }

                if (m_modulesEnabled & entityModules::RIGID_BODY && !m_static)
                    {
                        m_rigidBody = fe::engine::get().getPhysicsEngine().createRigidBody();
                    }

                if (m_modulesEnabled & entityModules::COLLISION_BODY)
                    {
                        m_collisionBody = fe::engine::get().getCollisionWorld().createCollider(0.f, 0.f);
                        m_collisionBody->m_static = m_static;
                    }

                setPosition(m_positionX, m_positionY);
                setSize(m_sizeX, m_sizeY);
                setColour(m_colour);

                enable(true);
            }

        // Render Object
        template<typename T>
        void fe::baseEntity::initRenderObject()
            {
                m_renderObject = fe::engine::get().getStateMachine().getSceneGraph().createRenderObject();
            }

        template<typename T0, typename ...Args, typename std::enable_if<std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type>
        void baseEntity::initRenderObject(T0 &&arg0, Args &&...args)
            {
                m_renderObject = fe::engine::get().getStateMachine().getSceneGraph().createRenderObject(arg0);
            }

        template<typename T0, typename ...Args, typename std::enable_if<!std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type>
        void baseEntity::initRenderObject(T0 &&arg0, Args &&...args)
            {
                FE_ASSERT(false, "You Should Never Get Here -- InitRenderObject");
            }

        // Render Text
        template<typename T>
        void fe::baseEntity::initRenderText()
            {
                m_renderObject = fe::engine::get().getStateMachine().getSceneGraph().createRenderTextObject();
            }

        template<typename T0, typename ...Args, typename std::enable_if<!std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type>
        void baseEntity::initRenderText(T0 &&arg0, Args &&...args)
            {
                m_renderObject = fe::engine::get().getStateMachine().getSceneGraph().createRenderTextObject(arg0, std::forward<Args>(args)...);
            }

        template<typename T0, typename ...Args, typename std::enable_if<std::is_integral<typename std::remove_reference<typename std::remove_pointer<T0>::type>::type>::value, int>::type>
        void baseEntity::initRenderText(T0 &&arg0, Args &&...args)
            {
                FE_ASSERT(false, "You Should Never Get Here -- InitRenderText");
            }

    }