// texturePacker.hpp
// a texture packer. Takes input textures, appends them to an output one and returns a data package
// Algorithm inspired from http://blackpawn.com/texts/lightmaps/default.html
#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "../../math/Vector2.hpp"

namespace fe
    {
        class texturePacker
            {
                private:
                    struct packNode
                        {
                            packNode *m_child[2];
                            sf::Texture *m_texture;

                            fe::Vector2<unsigned int> m_position;
                            fe::Vector2<unsigned int> m_size;
                    

                            packNode(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
                            packNode *insert(sf::Texture *texture);
                            void clear();

                        } m_baseNode;

                    sf::Texture m_packedTexture;
                    bool m_needsUpdate;

                    void createImage(packNode *node, sf::Image &packed);

                public:
                    texturePacker(fe::Vector2<unsigned int> textureSize = fe::Vector2<unsigned int>(2048, 2048));
                    void createTexture();

                    fe::Vector2<unsigned int> addTexture(sf::Texture *texture);
                    const sf::Texture &getTexture();

                    ~texturePacker();

            };
    }