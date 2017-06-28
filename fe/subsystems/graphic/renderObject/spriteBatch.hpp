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
        class transformable;
        class spriteBatch
            {
                private:
                    sf::Vertex m_batch[FE_MAX_GAME_OBJECTS * 4];

                public:
                    FLAT_ENGINE_API spriteBatch();

                    FLAT_ENGINE_API void add(const fe::renderObject *entity, fe::transformable &transform, unsigned int &index);
                    FLAT_ENGINE_API void clear();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app, sf::RenderStates states, unsigned int objectCount);

            };
    }
