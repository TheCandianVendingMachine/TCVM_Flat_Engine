#include "fe/subsystems/graphic/tileMap.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/resourceManager/resourceManager.hpp"
#include "fe/subsystems/collision/collisionWorld.hpp"
#include "fe/engineEvents.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/subsystems/gameState/gameWorld.hpp"
#include "fe/subsystems/collision/broadphaseAbstract.hpp"
#include "fe/math/signOf.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

void fe::tileMap::onAdd(fe::imp::tileWorld *object, fe::Handle objectHandle)
    {
        if ((object->colliderSizeX + object->colliderSizeY) > 0.f)
            {
                object->colliderPtr = fe::engine::get().getCollisionWorld().createCollider(object->colliderSizeX, object->colliderSizeY);
                object->colliderPtr->m_aabb.m_offsetX = object->colliderOffsetX;
                object->colliderPtr->m_aabb.m_offsetY = object->colliderOffsetY;
                object->colliderPtr->m_aabb.m_globalPositionX = object->xPosition;
                object->colliderPtr->m_aabb.m_globalPositionY = object->yPosition;
                object->colliderPtr->m_collisionGroup = object->collisionGroup;
            }
        object->handle = objectHandle;

        if (m_world.getDynamicBroadphase() && object->colliderPtr)
            {
                m_world.getDynamicBroadphase()->add(object->colliderPtr);
            }

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

        if (object->colliderPtr) 
            {
                if (m_world.getDynamicBroadphase())
                    {
                        m_world.getDynamicBroadphase()->remove(object->colliderPtr);
                    }

                fe::engine::get().getCollisionWorld().deleteCollider(object->colliderPtr);
            }
    }

fe::tileMap::tileMap(fe::gameWorld &world) :
    m_fabricationFilepath{"\0"},
    m_textureName{"\0"},
    m_world(world)
    {
    }

void fe::tileMap::updateTile(fe::Handle handle)
    {
        fe::imp::tileWorld *tile = getObject(handle);
        fe::imp::tile *tilePrefab = &m_fabrications[tile->prefabIndex];
        auto size = fe::Vector2<unsigned int>(tilePrefab->xSize, tilePrefab->ySize);
        auto pos = fe::Vector2d(tile->xPosition, tile->yPosition);
        auto texturePos = fe::Vector2d(static_cast<float>(tilePrefab->xTexturePosition), static_cast<float>(tilePrefab->yTexturePosition)) + m_textureOffset;

        m_verticies[tile->tilemapIndex + 0].position = fe::Vector2d(pos.x,               pos.y).convertToSfVec2();
        m_verticies[tile->tilemapIndex + 1].position = fe::Vector2d(pos.x + size.x,      pos.y).convertToSfVec2();
        m_verticies[tile->tilemapIndex + 2].position = fe::Vector2d(pos.x + size.x,      pos.y + size.y).convertToSfVec2();
        m_verticies[tile->tilemapIndex + 3].position = fe::Vector2d(pos.x,               pos.y + size.y).convertToSfVec2();
        
        m_verticies[tile->tilemapIndex + tile->textureIndex[0]].texCoords = fe::Vector2d(texturePos.x, texturePos.y).convertToSfVec2();
        m_verticies[tile->tilemapIndex + tile->textureIndex[1]].texCoords = fe::Vector2d(texturePos.x + size.x, texturePos.y).convertToSfVec2();
        m_verticies[tile->tilemapIndex + tile->textureIndex[2]].texCoords = fe::Vector2d(texturePos.x + size.x, texturePos.y + size.y).convertToSfVec2();
        m_verticies[tile->tilemapIndex + tile->textureIndex[3]].texCoords = fe::Vector2d(texturePos.x, texturePos.y + size.y).convertToSfVec2();

        if (m_world.getDynamicBroadphase() && tile->colliderPtr)
            {
                tile->colliderPtr->m_aabb.m_globalPositionX = tile->xPosition;
                tile->colliderPtr->m_aabb.m_globalPositionY = tile->yPosition;

                m_world.getDynamicBroadphase()->update(tile->colliderPtr);
            }
    }

void fe::tileMap::rotateTile(fe::Handle handle, int direction)
    {
        fe::imp::tileWorld *tile = getObject(handle);
        const unsigned int dirMod4 = (tile->rotation + direction) % 4;

        tile->rotation = dirMod4;

        // 4 is how many rotations are possible
        tile->textureIndex[0] = (dirMod4 + 0) % 4;
        tile->textureIndex[1] = (dirMod4 + 1) % 4;
        tile->textureIndex[2] = (dirMod4 + 2) % 4;
        tile->textureIndex[3] = (dirMod4 + 3) % 4;

        if (m_world.getDynamicBroadphase() && tile->colliderPtr)
            {
                fe::imp::tile *tilePrefab = &m_fabrications[tile->prefabIndex];
                unsigned int tileSizeOnTwoX = std::ceilf(static_cast<float>(tilePrefab->xSize) / 2.f);
                unsigned int tileSizeOnTwoY = std::ceilf(static_cast<float>(tilePrefab->ySize) / 2.f);

                float originalSizeX = tile->colliderSizeX;
                float originalSizeY = tile->colliderSizeY;
                float offsetX = tile->colliderOffsetX;
                float offsetY = tile->colliderOffsetY;

                const int sign = fe::signOf(direction);

                tile->colliderSizeX = -sign * originalSizeY;
                tile->colliderSizeY = sign * originalSizeX;
                tile->colliderOffsetX = -sign * (offsetY - tileSizeOnTwoY) + tileSizeOnTwoX;
                tile->colliderOffsetY = sign * (offsetX - tileSizeOnTwoX) + tileSizeOnTwoY;

                tile->colliderPtr->m_aabb.m_sizeX = tile->colliderSizeX;
                tile->colliderPtr->m_aabb.m_sizeY = tile->colliderSizeY;
                tile->colliderPtr->m_aabb.m_offsetX = tile->colliderOffsetX;
                tile->colliderPtr->m_aabb.m_offsetY = tile->colliderOffsetY;

                if (tile->colliderSizeX < 0.f)
                    {
                        tile->colliderPtr->m_aabb.m_offsetX += tile->colliderSizeX;
                        tile->colliderPtr->m_aabb.m_sizeX *= -1.f;
                    }

                if (tile->colliderSizeY < 0.f)
                    {
                        tile->colliderPtr->m_aabb.m_offsetY += tile->colliderSizeY;
                        tile->colliderPtr->m_aabb.m_sizeY *= -1.f;
                    }

                m_world.getDynamicBroadphase()->update(tile->colliderPtr);
            }
    }

void fe::tileMap::rebuildTilemap()
    {
        m_verticies.clear();
        m_verticies.resize(getObjects().size() * 4);
        m_verticies.setPrimitiveType(sf::PrimitiveType::Quads);

        int index = 0;
        for (auto &tileHandle : m_objects)
            {
                tileHandle.tilemapIndex = index;

                fe::imp::tile *tile = &m_fabrications[tileHandle.prefabIndex];
                auto size = fe::Vector2<unsigned int>(tile->xSize, tile->ySize);
                auto texturePos = fe::Vector2d(static_cast<float>(tile->xTexturePosition), static_cast<float>(tile->yTexturePosition)) + m_textureOffset;
                auto pos = fe::Vector2d(tileHandle.xPosition, tileHandle.yPosition);
                auto textureIndex = tileHandle.textureIndex;

                m_verticies[index + 0].position = fe::Vector2d(pos.x,               pos.y).convertToSfVec2();
                m_verticies[index + 1].position = fe::Vector2d(pos.x + size.x,      pos.y).convertToSfVec2();
                m_verticies[index + 2].position = fe::Vector2d(pos.x + size.x,      pos.y + size.y).convertToSfVec2();
                m_verticies[index + 3].position = fe::Vector2d(pos.x,               pos.y + size.y).convertToSfVec2();

                m_verticies[index + textureIndex[0]].texCoords = fe::Vector2d(texturePos.x,           texturePos.y).convertToSfVec2();
                m_verticies[index + textureIndex[1]].texCoords = fe::Vector2d(texturePos.x + size.x,  texturePos.y).convertToSfVec2();
                m_verticies[index + textureIndex[2]].texCoords = fe::Vector2d(texturePos.x + size.x,  texturePos.y + size.y).convertToSfVec2();
                m_verticies[index + textureIndex[3]].texCoords = fe::Vector2d(texturePos.x,           texturePos.y + size.y).convertToSfVec2();

                index += 4;
            }

        fe::engine::get().getEventSender().sendEngineEvent(fe::gameEvent(fe::engineEvent::TILE_MAP_TEXTURE_CHANGED, 0), fe::engineEvent::TILE_MAP_TEXTURE_CHANGED);
    }

void fe::tileMap::addGlobalTexture(const std::string &textureName)
    {
        addGlobalTexture(textureName.c_str());
    }

void fe::tileMap::addGlobalTexture(const char *textureName)
    {
        std::strcpy(m_textureName, textureName);
        m_textureOffset = fe::engine::get().getResourceManager<sf::Texture>()->getTexturePosition(m_textureName);
        rebuildTilemap();
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
        return *getObject(handle);
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

void fe::tileMap::create(const char *name, fe::Vector2<unsigned int> size, fe::Vector2<unsigned int> offset, fe::Vector2d colliderSize, fe::Vector2d colliderOffset)
    {
        imp::tile fab;

        std::strcpy(fab.id, name);
        fab.xTexturePosition = offset.x;
        fab.yTexturePosition = offset.y;
        fab.xSize = size.x;
        fab.ySize = size.y;

        fab.collider.m_aabb.m_sizeX = colliderSize.x;
        fab.collider.m_aabb.m_sizeY = colliderSize.y;
        fab.collider.m_aabb.m_offsetX = colliderOffset.x;
        fab.collider.m_aabb.m_offsetY = colliderOffset.y;

        create(fab);
    }

void fe::tileMap::create(fe::imp::tile &tile)
    {
        m_fabrications.push_back(tile);
    }

fe::Handle fe::tileMap::add(fe::Vector2d position, fe::str tileId)
    {
        auto it = std::find_if(m_fabrications.begin(), m_fabrications.end(), [tileId](fe::imp::tile &tile) { return FE_STR(tile.id) == tileId; });
        if (it != m_fabrications.end())
            {
                fe::imp::tile &tile = *it;

                // Im adding the position to the prefab since this will be copied to the new element, and this will be overwritten and not used
                fe::imp::tileWorld tileWorld;
                tileWorld.xPosition = position.x;
                tileWorld.yPosition = position.y;
                tileWorld.id = FE_STR(tile.id);
                tileWorld.colliderSizeX = tile.collider.m_aabb.m_sizeX;
                tileWorld.colliderSizeY = tile.collider.m_aabb.m_sizeY;
                tileWorld.colliderOffsetX = tile.collider.m_aabb.m_offsetX;
                tileWorld.colliderOffsetY = tile.collider.m_aabb.m_offsetY;
                tileWorld.collisionGroup = tile.collider.m_collisionGroup;
                tileWorld.textureIndex[0] = 0;
                tileWorld.textureIndex[1] = 1;
                tileWorld.textureIndex[2] = 2;
                tileWorld.textureIndex[3] = 3;
                tileWorld.rotation = 0;
                tileWorld.prefabIndex = it - m_fabrications.begin();
                fe::Handle retHandle = addObject(tileWorld);
                rebuildTilemap();
                return retHandle;
            }

        return -1;
    }

fe::imp::tileWorld &fe::tileMap::get(fe::Handle handle)
    {
        return *getObject(handle);
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

void fe::tileMap::serialize(fe::serializerID &serializer) const
    {
        for (auto &tile : m_objects)
            {
                serializer.writeObjectList("tiles", tile);
            }
        serializer.write("textureName", m_textureName);
        serializer.write("fabricationPath", m_fabricationFilepath);
    }

void fe::tileMap::deserialize(fe::serializerID &serializer)
    {
        addGlobalTexture(serializer.read<std::string>("textureName"));
        loadFabrications(serializer.read<std::string>("fabricationPath").c_str());

        while (serializer.listHasItems("tiles"))
            {
                fe::imp::tileWorld obj;
                serializer.readObjectList("tiles", obj);

                auto it = std::find_if(m_fabrications.begin(), m_fabrications.end(), [obj](fe::imp::tile &tile) { return FE_STR(tile.id) == obj.id; });
                if (it != m_fabrications.end())
                    {
                        fe::imp::tile &tile = *it;
                        obj.id = FE_STR(tile.id);
                        obj.colliderSizeX = tile.collider.m_aabb.m_sizeX;
                        obj.colliderSizeY = tile.collider.m_aabb.m_sizeY;
                        obj.colliderOffsetX = tile.collider.m_aabb.m_offsetX;
                        obj.colliderOffsetY = tile.collider.m_aabb.m_offsetY;
                        obj.collisionGroup = tile.collider.m_collisionGroup;
                        obj.prefabIndex = it - m_fabrications.begin();
                        obj.textureIndex[0] = 0;
                        obj.textureIndex[1] = 1;
                        obj.textureIndex[2] = 2;
                        obj.textureIndex[3] = 3;
                        unsigned int rotation = obj.rotation;
                        obj.rotation = 0;
                        fe::Handle retHandle = addObject(obj);

                        for (unsigned int i = 0; i < rotation; i++)
                            {
                                rotateTile(retHandle, 1);
                            }
                    }
            }

        rebuildTilemap();
    }

void fe::tileMap::serializeFabrications(fe::serializerID &serializer) const
    {
        for (auto &fab : m_fabrications)
            {
                serializer.writeObjectList("fabrications", fab);
            }
    }

void fe::tileMap::deserializeFabrications(fe::serializerID &serializer)
    {
        while (serializer.listHasItems("fabrications"))
            {
                fe::imp::tile fab;
                serializer.readObjectList("fabrications", fab);
                create(fab);
            }
    }

void fe::tileMap::loadFabrications(const char *filepath)
    {
        clearFabs();
        std::strcpy(m_fabricationFilepath, filepath);

        std::ifstream load(filepath);
        fe::serializerID serial;
        serial.readData(load);
        load.close();

        deserializeFabrications(serial);
        rebuildTilemap();
    }

void fe::tileMap::saveFabrications(const char *filepath)
    {
        fe::serializerID serial;
        serializeFabrications(serial);

        std::ofstream save(filepath);
        serial.outData(save);
        save.close();
    }

const std::vector<fe::imp::tile> &fe::tileMap::getFabrications()
    {
        return m_fabrications;
    }

const std::vector<fe::imp::tileWorld> &fe::tileMap::getTiles() const
    {
        return getObjects();
    }

void fe::tileMap::clearMap()
    {
        m_verticies.clear();
        clearAllObjects();
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

void fe::imp::tileWorld::serialize(fe::serializerID &serializer) const
    {
        serializer.write("id", id);
        serializer.write("handle", handle);
        serializer.write("x", xPosition);
        serializer.write("y", yPosition);
        serializer.write("rotation", rotation);
        serializer.write("collisionGroup", collisionGroup);
    }

void fe::imp::tileWorld::deserialize(fe::serializerID &serializer)
    {
        id = serializer.read<fe::str>("id");
        handle = serializer.read<fe::Handle>("handle");
        xPosition = serializer.read<float>("x");
        yPosition = serializer.read<float>("y");
        rotation = serializer.read<unsigned int>("rotation");
        collisionGroup = serializer.read<unsigned int>("collisionGroup");
    }

void fe::imp::tile::serialize(fe::serializerID &serializer) const
    {
        serializer.write("xSize", xSize);
        serializer.write("ySize", ySize);
        serializer.write("xTexturePosition", xTexturePosition);
        serializer.write("yTexturePosition", yTexturePosition);
        serializer.write("id", id);
        serializer.writeObject("collider", collider);
    }

void fe::imp::tile::deserialize(fe::serializerID &serializer)
    {
        xSize = serializer.read<unsigned int>("xSize");
        ySize = serializer.read<unsigned int>("ySize");
        xTexturePosition = serializer.read<unsigned int>("xTexturePosition");
        yTexturePosition = serializer.read<unsigned int>("yTexturePosition");
        std::strcpy(id, serializer.read<std::string>("id").c_str());
        serializer.readObject("collider", collider);
    }
