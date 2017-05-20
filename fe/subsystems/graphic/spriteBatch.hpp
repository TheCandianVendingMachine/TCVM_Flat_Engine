// spriteBatch.hpp
// a small sprite batch to lower amount of draw calls that happen
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <SFML/Graphics/VertexArray.hpp>

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
                    sf::VertexArray m_batch;
                    unsigned int m_maxVertexCount; // optimization to resize the batch after every clear so we can avoid dynamic allocations
                    unsigned int m_lastVertexCount;

                    unsigned int m_index; // index of the current sprite
                    unsigned int m_lastEntityCount; // How many entities were processed last frame
                    unsigned int m_currentEntityCount; // How many entities were processed this frame

                    bool m_needsPurge; // If this is true, all entities will be re-added to the vertex array

                public:
                    FLAT_ENGINE_API spriteBatch();

                    FLAT_ENGINE_API void add(const sf::VertexArray &entity, fe::transformable &transform);
                    FLAT_ENGINE_API void clear();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app, sf::RenderStates states);

            };
    }
