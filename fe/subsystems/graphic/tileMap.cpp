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
        for (auto &tileHandle : getObjects())
            {
                fe::imp::tile *tile = &m_fabrications[tileHandle.handle];
                auto size = fe::Vector2d(tile->xSize, tile->ySize);
                auto texturePos = fe::Vector2d(tile->xTexturePosition, tile->yTexturePosition) + m_textureOffset;
                auto pos = fe::Vector2d(tileHandle.xPosition, tileHandle.yPosition);

                m_verticies[index + 0].position = fe::Vector2d(pos.x,               pos.y).convertToSfVec2();
                m_verticies[index + 1].position = fe::Vector2d(pos.x + size.x,      pos.y).convertToSfVec2();
                m_verticies[index + 2].position = fe::Vector2d(pos.x + size.x,      pos.y + size.y).convertToSfVec2();
                m_verticies[index + 3].position = fe::Vector2d(pos.x,               pos.y + size.y).convertToSfVec2();

                m_verticies[index + 0].texCoords = fe::Vector2d(texturePos.x,           texturePos.y).convertToSfVec2();
                m_verticies[index + 1].texCoords = fe::Vector2d(texturePos.x + size.x,  texturePos.y).convertToSfVec2();
                m_verticies[index + 2].texCoords = fe::Vector2d(texturePos.x + size.x,  texturePos.y + size.y).convertToSfVec2();
                m_verticies[index + 3].texCoords = fe::Vector2d(texturePos.x,           texturePos.y + size.y).convertToSfVec2();

                index += 4;
            }
    }

void fe::tileMap::addGlobalTexture(fe::Vector2<unsigned int> offset)
    {
        m_textureOffset = offset;
    }

fe::Vector2<unsigned int> fe::tileMap::getTextureOffset() const
    {
        return m_textureOffset;
    }

const fe::imp::tile *fe::tileMap::getTile(fe::str tileID) const
    {
        for (auto &tile : m_fabrications)
            {
                if (FE_STR(tile.id) == tileID)
                    {
                        return &tile;
                    }
            }
        return nullptr;
    }

fe::Vector2<unsigned int> fe::tileMap::getTileTextureOffset(fe::str tileID) const
    {
        for (auto &tile : m_fabrications)
            {
                if (FE_STR(tile.id) == tileID)
                    {
                        return fe::Vector2<unsigned int>(tile.xTexturePosition, tile.yTexturePosition);
                    }
            }
        return fe::Vector2<unsigned int>(0, 0);
    }

void fe::tileMap::create(const char *name, fe::Vector2<unsigned int> size, fe::Vector2<unsigned int> offset)
    {
        imp::tile fab;

        std::strcpy(fab.id, name);
        fab.xTexturePosition = offset.x;
        fab.yTexturePosition = offset.y;
        fab.xSize = size.x;
        fab.ySize = size.y;

        m_fabrications.push_back(fab);
    }

fe::Handle fe::tileMap::add(fe::Vector2d position, fe::str tileId)
    {
        int iteration = 0;
        for (auto &tile : m_fabrications)
            {
                if (FE_STR(tile.id) == tileId)
                    {
                        // Im adding the position to the prefab since this will be copied to the new element, and this will be overwritten and not used
                        fe::imp::tileWorld tileWorld;
                        tileWorld.xPosition = position.x;
                        tileWorld.yPosition = position.y;
                        tileWorld.id = FE_STR(tile.id);
                        tileWorld.handle = iteration;
                        tileWorld.str = fe::clock::getTimeSinceEpoch().asMicroseconds();
                        fe::Handle retHandle = addObject(tileWorld);
                        rebuildTilemap();
                        return retHandle;
                        break;
                    }
                ++iteration;
            }

        return -1;
    }

void fe::tileMap::remove(fe::Handle handle)
    {
        removeObject(handle);
        rebuildTilemap();
    }

fe::Handle fe::tileMap::get(fe::Vector2d position)
    {
        for (auto &tile : getObjects())
            {
                fe::Vector2d tPosition(tile.xPosition, tile.yPosition);
                fe::Vector2<unsigned int> tSize = fe::Vector2<unsigned int>(m_fabrications[tile.handle].xSize, m_fabrications[tile.handle].ySize);
                if (position.x >= tPosition.x && position.x < tPosition.x + tSize.x &&
                    position.y >= tPosition.y && position.y < tPosition.y + tSize.y)
                    {
                        return getHandle(tile);
                    }
            }

        return -1;
    }

void fe::tileMap::draw(sf::RenderTarget &target, sf::RenderStates states)
    {
        target.draw(m_verticies, states);
    }

void fe::tileMap::serializeFabrications(fe::serializerID &serial)
    {
        for (auto &fab : m_fabrications)
            {
                fab.serialize(serial);
            }
    }

void fe::tileMap::deserializeFabrications(fe::serializerID &serial)
    {
        clearFabs();
        imp::tile temp;
        while (temp.deserialize(serial))
            {
                m_fabrications.push_back(temp);
            }
    }

const std::vector<fe::imp::tile> &fe::tileMap::getFabrications()
    {
        return m_fabrications;
    }

void fe::tileMap::clearMap()
    {
        m_verticies.clear();
        getObjects().clear();
    }

void fe::tileMap::clearFabs()
    {
        m_fabrications.clear();
    }

void fe::tileMap::clear()
    {
        clearMap();
        clearFabs();
    }
