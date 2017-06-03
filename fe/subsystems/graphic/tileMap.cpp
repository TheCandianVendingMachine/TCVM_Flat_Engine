#include "tileMap.hpp"
#include "../../engine.hpp"
#include "../resourceManager/resourceManager.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::tileMap::rebuildTilemap()
    {
        m_verticies.clear();
        m_verticies.resize(getObjects().size() * 4);
        m_verticies.setPrimitiveType(sf::PrimitiveType::Quads);

        int index = 0;
        for (auto &tile : getObjects())
            {
                auto size = fe::engine::get().getResourceManager<sf::Texture>()->getTexture(tile.textureId)->getSize();
                auto pos = fe::Vector2d(tile.xPosition, tile.yPosition);
                auto offset = fe::engine::get().getResourceManager<sf::Texture>()->getTexturePosition(tile.textureId);

                m_verticies[index    ].position = fe::Vector2d(pos.x,           pos.y).convertToSfVec2();
                m_verticies[index + 1].position = fe::Vector2d(pos.x + size.x,  pos.y).convertToSfVec2();
                m_verticies[index + 2].position = fe::Vector2d(pos.x + size.x,  pos.y + size.y).convertToSfVec2();
                m_verticies[index + 3].position = fe::Vector2d(pos.x,           pos.y + size.y).convertToSfVec2();

                m_verticies[index    ].texCoords = fe::Vector2d(offset.x,           offset.y).convertToSfVec2();
                m_verticies[index + 1].texCoords = fe::Vector2d(offset.x + size.x,  offset.y).convertToSfVec2();
                m_verticies[index + 2].texCoords = fe::Vector2d(offset.x + size.x,  offset.y + size.y).convertToSfVec2();
                m_verticies[index + 3].texCoords = fe::Vector2d(offset.x,           offset.y + size.y).convertToSfVec2();

                index += 4;
            }
    }

void fe::tileMap::create(fe::guid name, fe::guid textureID)
    {
        imp::tile fab;

        fab.id = name;
        fab.textureId = textureID;

        m_fabrications.push_back(fab);
    }

fe::Handle fe::tileMap::add(fe::Vector2d position, fe::guid tileId)
    {
        for (auto &tile : m_fabrications)
            {
                if (tile.id == tileId)
                    {
                        // I assing the position to the prefab since this will be copied to the new element, and this will be overwritten and not used
                        tile.xPosition = position.x;
                        tile.yPosition = position.y;

                        fe::Handle retHandle = addObject(tile);
                        rebuildTilemap();
                        return retHandle;
                        break;
                    }
            }

        return -1;
    }

void fe::tileMap::remove(fe::Vector2d position, fe::Vector2d size)
    {
        for (auto &tile : getObjects())
            {
                fe::Vector2d tPosition(tile.xPosition, tile.yPosition);
                fe::Vector2<unsigned int> tSize = fe::engine::get().getResourceManager<sf::Texture>()->getTexture(tile.textureId)->getSize();
                if (tPosition.x >= position.x && tPosition.x + tSize.x < position.x + size.x &&
                    tPosition.y >= position.y && tPosition.y + tSize.y < position.y + size.y)
                    {
                        remove(getHandle(tile));
                        return;
                    }
            }
    }

void fe::tileMap::remove(fe::Handle handle)
    {
        removeObject(handle);
        rebuildTilemap();
    }

void fe::tileMap::draw(sf::RenderTarget &target, sf::RenderStates states)
    {
        target.draw(m_verticies, states);
    }

void fe::tileMap::serialize(fe::serializerID &serial)
    {
        for (auto &tile : getObjects())
            {
                tile.serialize(serial);
            }
    }

void fe::tileMap::deserialize(fe::serializerID &serial)
    {
        imp::tile temp;
        while (temp.deserialize(serial))
            {
                add({temp.xPosition, temp.yPosition}, temp.id);
            }
    }
