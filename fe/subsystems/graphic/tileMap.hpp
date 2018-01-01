// tileMap.hpp
// a tile map. Allows the user to load a tileset and draw tiles
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/str.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../../math/Vector2.hpp"
#include "../serializer/serializerID.hpp"
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
                struct tile
                    {
                        char id[64];

                        unsigned int xTexturePosition;
                        unsigned int yTexturePosition;
                        unsigned int xSize;
                        unsigned int ySize;

                        fe::collider collider;

                        SERIALIZE_ID("tile", "xSize", xSize, "ySize", ySize, "xTexturePosition", xTexturePosition, "yTexturePosition", yTexturePosition, "id", id, "collider", collider);
                    };

                struct tileWorld : public fe::guid
                    {
                        fe::str id;
                        int handle = -1;
                        float xPosition;
                        float yPosition;

                        float m_colliderSizeX;
                        float m_colliderSizeY;

                        fe::collider *colliderPtr = nullptr;

                        SERIALIZE_ID("tile", "id", id, "handle", handle, "x", xPosition, "y", yPosition);
                    };
            }

        class tileMap : private fe::handleManager<fe::imp::tileWorld, 0>
            {
                private:
                    std::vector<fe::imp::tile> m_fabrications; // tiles already defined
                    sf::VertexArray m_verticies;
                    fe::Vector2<unsigned int> m_textureOffset; // offset of texture in texture packer

                    char m_textureName[64];

                    FLAT_ENGINE_API void onAdd(fe::imp::tileWorld *object, fe::Handle objectHandle);
                    FLAT_ENGINE_API void onRemove(fe::imp::tileWorld *object, fe::Handle objectHandle);

                    FLAT_ENGINE_API void onSave() const;
                    FLAT_ENGINE_API void onLoad();

                public:
                    FLAT_ENGINE_API tileMap();

                    FLAT_ENGINE_API void rebuildTilemap();

                    FLAT_ENGINE_API void addGlobalTexture(const char *textureName);
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getTextureOffset() const;
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getTileTextureOffset(fe::str tileID) const;
                    FLAT_ENGINE_API const imp::tile *getPrefabTile(fe::str tileID) const;
                    FLAT_ENGINE_API const fe::imp::tileWorld &getPlacedTile(fe::Handle handle) const;

                    // Create a tile and put it into the fabrications
                    FLAT_ENGINE_API void create(const char *name, fe::Vector2<unsigned int> size, fe::Vector2<unsigned int> offset);

                    FLAT_ENGINE_API fe::Handle add(fe::Vector2d position, fe::str tileId);
                    FLAT_ENGINE_API imp::tileWorld &get(fe::Handle handle);
                    FLAT_ENGINE_API void remove(fe::Handle handle);
                    FLAT_ENGINE_API fe::Handle get(fe::Vector2d position);

                    FLAT_ENGINE_API void draw(sf::RenderTarget &target, sf::RenderStates states);

                    SERIALIZE_CALLBACK_ID(onSave(), onLoad(), "tilemap", "tiles", m_objects, "textureName", m_textureName);
                    SERIALIZE_NAME_ID(Fabrications, "tilemap", "fabrications", m_fabrications);

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