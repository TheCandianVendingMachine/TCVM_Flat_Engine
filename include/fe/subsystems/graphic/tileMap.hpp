// tileMap.hpp
// a tile map. Allows the user to load a tileset and draw tiles
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/objectManagement/str.hpp"
#include "fe/objectManagement/handleManager.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/subsystems/serializer/serializable.hpp"
#include "fe/objectManagement/guid.hpp"
#include "fe/subsystems/collision/collisionBody.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class gameWorld;
        namespace imp
            {
                struct tile : public fe::serializable
                    {
                        char id[64];

                        unsigned int xTexturePosition;
                        unsigned int yTexturePosition;
                        unsigned int xSize;
                        unsigned int ySize;

                        fe::collider collider;

                        FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                        FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
                    };

                struct tileWorld : public fe::guid, public fe::serializable
                    {
                        fe::str id;
                        fe::str collisionGroup;
                        fe::Handle handle;
                        int prefabIndex = -1;
                        unsigned int tilemapIndex;
                        unsigned int textureIndex[4];
                        unsigned int rotation;
                        float xPosition;
                        float yPosition;

                        // Temporary values used to initialize new tiles and tile rotation
                        float colliderOffsetX;
                        float colliderOffsetY;
                        float colliderSizeX;
                        float colliderSizeY;

                        fe::collider *colliderPtr = nullptr;

                        FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                        FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
                    };
            }

        class tileMap : private fe::handleManager<fe::imp::tileWorld, 0>, public fe::serializable
            {
                private:
                    std::vector<fe::imp::tile> m_fabrications; // tiles already defined
                    sf::VertexArray m_verticies;
                    fe::Vector2<unsigned int> m_textureOffset; // offset of texture in texture packer

                    fe::gameWorld &m_world;

                    char m_textureName[64];
                    char m_fabricationFilepath[128];

                    FLAT_ENGINE_API void onAdd(fe::imp::tileWorld *object, fe::Handle objectHandle);
                    FLAT_ENGINE_API void onRemove(fe::imp::tileWorld *object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API tileMap(fe::gameWorld &world);

                    FLAT_ENGINE_API void updateTile(fe::Handle handle);
                    // Sets the texture index to rotate the tile. Tiles needs to be updated in order to render as rotated
                    FLAT_ENGINE_API void rotateTile(fe::Handle handle, int direction);
                    FLAT_ENGINE_API void rebuildTilemap();

                    FLAT_ENGINE_API void addGlobalTexture(const std::string &textureName);
                    FLAT_ENGINE_API void addGlobalTexture(const char *textureName);
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getTextureOffset() const;
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getTileTextureOffset(fe::str tileID) const;
                    FLAT_ENGINE_API const imp::tile *getPrefabTile(fe::str tileID) const;
                    FLAT_ENGINE_API const fe::imp::tileWorld &getPlacedTile(fe::Handle handle) const;

                    // Create a tile and put it into the fabrications
                    FLAT_ENGINE_API void create(const char *name, fe::Vector2<unsigned int> size, fe::Vector2<unsigned int> offset, fe::Vector2d colliderSize, fe::Vector2d colliderOffset);
                    FLAT_ENGINE_API void create(fe::imp::tile &tile);

                    FLAT_ENGINE_API fe::Handle add(fe::Vector2d position, fe::str tileId);
                    FLAT_ENGINE_API fe::Handle addWithoutRebuild(fe::Vector2d position, fe::str tileId);
                    FLAT_ENGINE_API imp::tileWorld &get(fe::Handle handle);
                    FLAT_ENGINE_API void removeWithoutRebuild(fe::Handle handle);
                    FLAT_ENGINE_API void remove(fe::Handle handle);
                    FLAT_ENGINE_API fe::Handle get(fe::Vector2d position);

                    FLAT_ENGINE_API void draw(sf::RenderTarget &target, sf::RenderStates states);

                    FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                    FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

                    FLAT_ENGINE_API void serializeFabrications(fe::serializerID &serializer) const;
                    FLAT_ENGINE_API void deserializeFabrications(fe::serializerID &serializer);

                    FLAT_ENGINE_API void loadFabrications(const char *filepath);
                    FLAT_ENGINE_API void saveFabrications(const char *filepath);

                    // Returns a vector of all fabricated tiles
                    FLAT_ENGINE_API const std::vector<fe::imp::tile> &getFabrications();
                    FLAT_ENGINE_API const std::vector<fe::imp::tileWorld> &getTiles() const;

                    // Clears tilemap of all verticies
                    FLAT_ENGINE_API void clearMap();
                    // Clears all pre-fabricated tiles
                    FLAT_ENGINE_API void clearFabs();
                    // Clears all data
                    FLAT_ENGINE_API void clear();

            };
    }