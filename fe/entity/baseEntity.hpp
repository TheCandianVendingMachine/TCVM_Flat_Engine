// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "transformable.hpp"
#include "drawable.hpp"
#include "../collision/colliders.hpp"

#include "../objectManagement/handleManager.hpp"

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class baseGameState;

        struct collider;
        class baseEntity : public fe::transformable, public fe::drawable
            {
                private:
                    fe::collider *m_collider;

                    fe::Handle m_handle;
                    bool m_destroyed;

                protected:
                    FLAT_ENGINE_API void addCollider(fe::collider *collider);

                public:
                    FLAT_ENGINE_API baseEntity();
                    FLAT_ENGINE_API virtual ~baseEntity();

                    // Set the objects handle. DO NOT CALL IF YOU DONT KNOW WHAT YOU ARE DOING
                    FLAT_ENGINE_API void setHandle(fe::Handle handle);

                    FLAT_ENGINE_API void enableCollision(bool value);

                    // Destroy the entity. Entity will be removed from the current scene
                    FLAT_ENGINE_API void destroy();
                    // Returns if the object is destroyed
                    FLAT_ENGINE_API bool isDestroyed() const;
                    // Called when the entity is destroyed
                    FLAT_ENGINE_API void onDestroy(fe::baseGameState &state);

                    FLAT_ENGINE_API virtual void onAdd(baseGameState &state) {}
                    FLAT_ENGINE_API virtual void onRemove(baseGameState &state) {}

                    // update functions
                    FLAT_ENGINE_API virtual void update(float deltaTime) = 0;

                    FLAT_ENGINE_API fe::collider &getCollider() const;
                    FLAT_ENGINE_API bool hasCollider() const;

                    FLAT_ENGINE_API fe::Handle getHandle() const;

                    FLAT_ENGINE_API virtual void collision(fe::collisionData testCollider) {}

                    FLAT_ENGINE_API fe::Vector2d getSize() const;
            };
    }