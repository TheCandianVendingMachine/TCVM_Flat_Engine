// tileMap.hpp
// a tile map. Allows the user to load a tileset and draw tiles
#pragma once
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/str.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../../math/Vector2.hpp"
#include "../serializer/serializable.hpp"
#include "../../objectManagement/guid.hpp"
#include "../physics/collision/collisionBody.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
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
                        int handle = -1;
                        float xPosition;
                        float yPosition;

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

                    char m_textureName[64];
                    char m_fabricationFilepath[128];

                    FLAT_ENGINE_API void onAdd(fe::imp::tileWorld *object, fe::Handle objectHandle);
                    FLAT_ENGINE_API void onRemove(fe::imp::tileWorld *object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API tileMap();

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
                    FLAT_ENGINE_API imp::tileWorld get(fe::Handle handle);
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

                    // Clears tilemap of all verticies
                    FLAT_ENGINE_API void clearMap();
                    // Clears all pre-fabricated tiles
                    FLAT_ENGINE_API void clearFabs();
                    // Clears all data
                    FLAT_ENGINE_API void clear();

            };
    }