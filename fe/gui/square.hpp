// square.hpp
// a square. Can have a texture mapped to it or not
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "guiElement.hpp"
#include <SFML/Graphics/Texture.hpp>

namespace fe
    {
        namespace gui
            {
                class square : public fe::gui::guiElement
                    {
                        private:
                            const sf::Texture *m_texture;

                            FLAT_ENGINE_API void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            FLAT_ENGINE_API square();
                            FLAT_ENGINE_API square(fe::Vector2d size, sf::Color colour = sf::Color::White);
                            FLAT_ENGINE_API square(const sf::Texture *texture, fe::Vector2d texCoords = {0, 0});

                    };
            }
    }