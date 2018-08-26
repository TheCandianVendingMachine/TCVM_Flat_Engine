// spriteBatch.hpp
// a small sprite batch to lower amount of draw calls that happen
#pragma once
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
                    static constexpr fe::uInt64 m_BATCH_SIZE = (FE_MAX_GAME_OBJECTS * 4) + ((FE_MAX_STRING_RENDER_SIZE * 4) * FE_MAX_TEXT_OBJECTS);
                    sf::Vertex m_batch[m_BATCH_SIZE];

                public:
                    FLAT_ENGINE_API spriteBatch();

                    FLAT_ENGINE_API void add(fe::renderObject *entity, unsigned int &index);
                    FLAT_ENGINE_API void add(fe::renderText *text, unsigned int &index);
                    FLAT_ENGINE_API void clear();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app, sf::RenderStates states, unsigned int vertexCount);

            };
    }
