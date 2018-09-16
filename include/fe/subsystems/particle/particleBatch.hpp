// particleBatch.hpp
// batched particle verticies
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/math/Vector2.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <vector>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class particleBatch
            {
                private:
                    std::vector<sf::Vertex> m_verticies;
                    unsigned int m_maxIndex;

                public:
                    FLAT_ENGINE_API void reset(unsigned int size);
                    FLAT_ENGINE_API void update(unsigned int index, float x, float y, float radius, sf::Color colour);
                    FLAT_ENGINE_API void draw(sf::RenderTarget &target);

            };
    }
