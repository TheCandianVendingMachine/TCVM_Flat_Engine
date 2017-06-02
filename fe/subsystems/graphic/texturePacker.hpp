// texturePacker.hpp
// a texture packer. Takes input textures, appends them to an output one and returns a data package
// Algorithm inspired from http://blackpawn.com/texts/lightmaps/default.html
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

#include <SFML/Graphics/Texture.hpp>
#include "../../math/Vector2.hpp"
#include "../../objectManagement/guid.hpp"

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

                            fe::guid m_id;
                    

                            FLAT_ENGINE_API packNode(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
                            FLAT_ENGINE_API packNode *insert(sf::Texture &texture);
                            FLAT_ENGINE_API packNode *get(const char *id);
                            FLAT_ENGINE_API void clear();

                        } m_baseNode;

                    sf::Texture m_packedTexture;
                    sf::Texture m_baseTexture; // a small white pixel used for non-texture objects
                    bool m_needsUpdate;

                    FLAT_ENGINE_API void createImage(packNode *node, sf::Image &packed);

                public:
                    FLAT_ENGINE_API texturePacker(fe::Vector2<unsigned int> textureSize = fe::Vector2<unsigned int>(2048, 2048));
                    FLAT_ENGINE_API void createTexture();

                    FLAT_ENGINE_API fe::Vector2<unsigned int> addTexture(sf::Texture &texture, const char *id);
                    FLAT_ENGINE_API const sf::Texture &getTexture();
                    FLAT_ENGINE_API const sf::Texture *getTexture(const char *id);
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getTexturePosition(const char *id);

                    FLAT_ENGINE_API ~texturePacker();

            };
    }