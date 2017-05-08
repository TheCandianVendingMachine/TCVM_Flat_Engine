#include "texturePacker.hpp"
#include <SFML/Graphics/Image.hpp>
#include <algorithm>

void fe::texturePacker::createImage(packNode *node, sf::Image &packed)
    {
        if (node->m_child[0])
            {
                createImage(node->m_child[0], packed);
                createImage(node->m_child[1], packed);
            }
        else if (node->m_texture)
            {
                packed.copy(node->m_texture->copyToImage(), node->m_position.x, node->m_position.y);
            }
    }

fe::texturePacker::texturePacker(fe::Vector2<unsigned int> textureSize) : m_needsUpdate(false), m_baseNode(0, 0, textureSize.x, textureSize.y)
    {
        m_packedTexture.create(textureSize.x, textureSize.y);
    }

void fe::texturePacker::createTexture()
    {
        sf::Image packed = m_packedTexture.copyToImage();
        createImage(&m_baseNode, packed);
        m_packedTexture.loadFromImage(packed);
    }

fe::Vector2<unsigned int> fe::texturePacker::addTexture(sf::Texture *texture)
    {
        packNode *ret = m_baseNode.insert(texture);
        if (ret) 
            {
                m_needsUpdate = true;
                return ret->m_position;
            }
        else
            {
                return fe::Vector2<unsigned int>(0, 0);
            }
    }

const sf::Texture &fe::texturePacker::getTexture()
    {
        if (m_needsUpdate)
            {
                createTexture();
            }
        
        return m_packedTexture;
    }

fe::texturePacker::~texturePacker()
    {
        m_baseNode.clear();
    }

fe::texturePacker::packNode::packNode(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        m_child[0] = nullptr;
        m_child[1] = nullptr;
        m_texture = nullptr;

        m_position = fe::Vector2<unsigned int>(x, y);
        m_size = fe::Vector2<unsigned int>(width, height);
    }

fe::texturePacker::packNode *fe::texturePacker::packNode::insert(sf::Texture *texture)
    {
        if (m_child[0]) // if we are a branch we will have atleast 1 child
            {
                packNode *newNode = m_child[0]->insert(texture);
                if (newNode) return newNode;

                return m_child[1]->insert(texture);
            }
        else
            {
                // if we already have a texture, return
                if (m_texture) return nullptr;
                
                // if we are too small we cant use this
                if (texture->getSize().x > m_size.x || texture->getSize().y > m_size.y)
                    {
                        return nullptr;
                    }
                // if we are the perfect size, that means we just generated this as the node that holds the texture
                else if (texture->getSize().x == m_size.x && texture->getSize().y == m_size.y)
                    {
                        m_texture = texture;
                        return this;
                    }

                float dWidth = m_size.x - texture->getSize().x;
                float dHeight = m_size.y - texture->getSize().y;

                if (dWidth > dHeight)
                    {
                        // packing into columns > ||
                        m_child[0] = new packNode(m_position.x,
                                                  m_position.y,
                                                  texture->getSize().x,
                                                  m_size.y);


                        m_child[1] = new packNode(m_position.x + texture->getSize().x,
                                                  m_position.y,
                                                  m_size.x - texture->getSize().x,
                                                  m_size.y);
                    }
                else
                    {
                        // packing into rows > =
                        m_child[0] = new packNode(m_position.x,
                                                  m_position.y,
                                                  m_size.x,
                                                  texture->getSize().y);

                        m_child[1] = new packNode(m_position.x,
                                                  m_position.y + texture->getSize().y,
                                                  m_size.x,
                                                  m_size.y - texture->getSize().y);
                    }

                return m_child[0]->insert(texture);
            }
    }

void fe::texturePacker::packNode::clear()
    {
        if (m_child[0])
            {
                m_child[0]->clear();
                m_child[1]->clear();

                delete m_child[0];
                delete m_child[1];

                m_child[0] = nullptr;
                m_child[1] = nullptr;
            }
    }
