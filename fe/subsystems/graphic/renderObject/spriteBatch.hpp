// spriteBatch.hpp
// a small sprite batch to lower amount of draw calls that happen
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "renderObject.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <mutex>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        struct matrix3d;
        class spriteBatch
            {
                private:
                    sf::Vertex m_batch[FE_MAX_GAME_OBJECTS * 4];

                public:
                    FLAT_ENGINE_API spriteBatch();

                    FLAT_ENGINE_API void add(fe::renderObject *entity, unsigned int &index);
                    FLAT_ENGINE_API void clear();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app, sf::RenderStates states, unsigned int objectCount);

            };
    }
