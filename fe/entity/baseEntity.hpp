// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "transformable.hpp"
#include "drawable.hpp"

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class baseEntity : public fe::transformable, public fe::drawable
            {
                public:
                    FLAT_ENGINE_API baseEntity();
                    FLAT_ENGINE_API virtual ~baseEntity();

                    // update functions
                    FLAT_ENGINE_API virtual void update(float deltaTime) = 0;

            };
    }