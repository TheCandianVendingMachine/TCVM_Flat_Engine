// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../typeDefines.hpp"
#include "../subsystems/physics/rigidBody.hpp"
#include "../subsystems/physics/collision/collisionBody.hpp"
#include <SFML/Graphics/Color.hpp>

namespace fe
    {
        struct renderObject;
        class rigidBody;
        class baseGameState;

        enum class entityModules : std::int16_t
            {
                RENDER_OBJECT   = 1 << 0,
                RIGID_BODY      = 1 << 1,
                COLLISION_BODY  = 1 << 2,
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
                    fe::renderObject *m_renderObject;
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

                public:
                    FLAT_ENGINE_API baseEntity(entityModules modules, bool staticObject);
                    FLAT_ENGINE_API void initialize();
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

                    FLAT_ENGINE_API fe::renderObject *getRenderObject() const;
                    FLAT_ENGINE_API fe::rigidBody *getRigidBody() const;
                    FLAT_ENGINE_API fe::collider *getCollider() const;

                    FLAT_ENGINE_API fe::Handle GUID() const;

                    void move() { m_rigidBody->applyForce(-2500, 0); }

            };
    }