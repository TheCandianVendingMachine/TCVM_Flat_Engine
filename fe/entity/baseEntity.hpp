// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "transformable.hpp"
#include "drawable.hpp"
#include "../collision/colliders.hpp"

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

                protected:
                    FLAT_ENGINE_API void addCollider(fe::collider *collider);

                public:
                    FLAT_ENGINE_API baseEntity();
                    FLAT_ENGINE_API virtual ~baseEntity();

                    FLAT_ENGINE_API virtual void onAdd(baseGameState &state) {}
                    FLAT_ENGINE_API virtual void onRemove(baseGameState &state) {}

                    // update functions
                    FLAT_ENGINE_API virtual void update(float deltaTime) = 0;

                    FLAT_ENGINE_API fe::collider &getCollider() const;
                    FLAT_ENGINE_API virtual void collision(const fe::collider &testCollider) {}
            };
    }