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

                public:
                    FLAT_ENGINE_API spriteBatch();

                    FLAT_ENGINE_API void add(const sf::VertexArray &entity, fe::transformable &transform);
                    FLAT_ENGINE_API void clear();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

            };
    }
