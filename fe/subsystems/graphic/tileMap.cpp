#include "tileMap.hpp"
#include "../../engine.hpp"
#include "../resourceManager/resourceManager.hpp"
#include "../physics/collision/collisionWorld.hpp"
#include "../../engineEvents.hpp"
#include "../messaging/eventSender.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

void fe::tileMap::onAdd(fe::imp::tileWorld *object, fe::Handle objectHandle)
    {
        object->colliderPtr = fe::engine::get().getCollisionWorld().createCollider(object->m_colliderSizeX, object->m_colliderSizeY);
        object->colliderPtr->m_aabb.m_positionX = object->xPosition;
        object->colliderPtr->m_aabb.m_positionY = object->yPosition;
        object->colliderPtr->m_aabb.m_globalPositionX = object->xPosition;
        object->colliderPtr->m_aabb.m_globalPositionY = object->yPosition;

        fe::gameEvent event(fe::engineEvent::TILE_PLACED, 1);
        event.args[0].arg.TYPE_VOIDP = object;
        event.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
        fe::engine::get().getEventSender().sendEngineEvent(event, fe::engineEvent::TILE_PLACED);
    }

void fe::tileMap::onRemove(fe::imp::tileWorld *object, fe::Handle objectHandle)
    {
        fe::gameEvent event(fe::engineEvent::TILE_REMOVED, 1);
        event.args[0].arg.TYPE_VOIDP = object;
        event.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
        fe::engine::get().getEventSender().sendEngineEvent(event, fe::engineEvent::TILE_REMOVED);
        fe::engine::get().getCollisionWorld().deleteCollider(object->colliderPtr);
    }

void fe::tileMap::onSave() const
    {}

void fe::tileMap::onLoad()
    {
        addGlobalTexture(m_textureName);
        std::vector<fe::imp::tileWorld> objCopy;
        std::copy(m_objects.begin(), m_objects.end(), std::back_inserter(objCopy));
        m_objects.clear();

        for (auto &obj : objCopy)
            {
                add({ obj.xPosition, obj.yPosition }, obj.id);
            }

        rebuildTilemap();
    }

fe::tileMap::tileMap()
    {
    }

void fe::tileMap::rebuildTilemap()
    {
        m_verticies.clear();
        m_verticies.resize(getObjects().size() * 4);
        m_verticies.setPrimitiveType(sf::PrimitiveType::Quads);

        int index = 0;
        for (auto &tileHandle : getObjects())
            {
                if (tileHandle.handle < 0)
                    {
                        // slow to do, but will get the right handle if not loaded
                        for (unsigned int i = 0; i < m_fabrications.size(); i++)
                            {
                                if (FE_STR(m_fabrications[i].id) == tileHandle.id)
                                    {
                                        tileHandle.handle = i;
                                        break;
                                    }
                            }
                    }

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

void fe::tileMap::addGlobalTexture(const char *textureName)
    {
        std::strcpy(m_textureName, textureName);
        m_textureOffset = fe::engine::get().getResourceManager<sf::Texture>()->getTexturePosition(m_textureName);
    }

fe::Vector2<unsigned int> fe::tileMap::getTextureOffset() const
    {
        return m_textureOffset;
    }

const fe::imp::tile *fe::tileMap::getPrefabTile(fe::str tileID) const
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

const fe::imp::tileWorld &fe::tileMap::getPlacedTile(fe::Handle handle) const
    {
        return getObject(handle);
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
        for (auto &tile : m_fabrications)
            {
                if (FE_STR(tile.id) == tileId)
                    {
                        // Im adding the position to the prefab since this will be copied to the new element, and this will be overwritten and not used
                        fe::imp::tileWorld tileWorld;
                        tileWorld.xPosition = position.x;
                        tileWorld.yPosition = position.y;
                        tileWorld.id = FE_STR(tile.id);
                        tileWorld.m_colliderSizeX = tile.xSize;
                        tileWorld.m_colliderSizeY = tile.ySize;
                        fe::Handle retHandle = addObject(tileWorld);
                        rebuildTilemap();
                        return retHandle;
                        break;
                    }
            }

        return -1;
    }

fe::imp::tileWorld &fe::tileMap::get(fe::Handle handle)
    {
        return getObject(handle);
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
