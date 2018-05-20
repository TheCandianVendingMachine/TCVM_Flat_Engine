// guiBatch.hpp
// a render batch for GUI elements. Excludes text objects
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <vector>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        struct polygon2d;
        class transformable;

        namespace gui
            {
                class guiBatch
                    {
                        private:
                            std::vector<sf::Vertex> m_batch;

                            unsigned int m_vertexIndex;
                            unsigned int m_vertexCount;

                        public:
                            FLAT_ENGINE_API guiBatch();

                            FLAT_ENGINE_API void add(const polygon2d &polygon, sf::Color drawColour, fe::transformable &transform);
                            FLAT_ENGINE_API void clear();
                            FLAT_ENGINE_API void draw(sf::RenderTarget &app, sf::RenderStates states);
                    };
            }
    }

