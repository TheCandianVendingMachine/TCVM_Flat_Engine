// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../typeDefines.hpp"
#include "../subsystems/physics/rigidBody.hpp"
#include "../subsystems/physics/collision/collisionBody.hpp"

namespace fe
    {
        struct renderObject;
        class rigidBody;
        class baseGameState;

        class baseEntity
            {
                protected:
                    fe::renderObject *m_renderObject;
                    fe::rigidBody *m_rigidBody;
                    fe::collider *m_collisionBody;

                    fe::Handle m_handle;

                    bool m_killEntity;
                    bool m_enabled;

                public:
                    FLAT_ENGINE_API baseEntity();

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

                    void move() { m_rigidBody->applyForce(-2500, 0); }

            };
    }