// tileMap.hpp
// a tile map. Allows the user to load a tileset and draw tiles
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/guid.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../../math/Vector2.hpp"
#include "../serializer/serializerID.hpp"
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
                        fe::guid id;

                        float xPosition;
                        float yPosition;

                        unsigned int xTexturePosition;
                        unsigned int yTexturePosition;
                        unsigned int xSize;
                        unsigned int ySize;

                        SERIALIZE_ID("tile", "x", xPosition, "y", yPosition, "xSize", xSize, "ySize", ySize, "xTexturePosition", xTexturePosition, "yTexturePosition", yTexturePosition, "id", id);
                    };
            }

        class tileMap : private fe::handleManager<fe::imp::tile*, 0>
            {
                private:
                    std::vector<fe::imp::tile> m_fabrications; // tiles already defined
                    sf::VertexArray m_verticies;
                    fe::Vector2<unsigned int> m_textureOffset; // offset of texture in texture packer

                    FLAT_ENGINE_API void rebuildTilemap();

                public:
                    FLAT_ENGINE_API void addGlobalTexture(fe::Vector2<unsigned int> offset);
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getTextureOffset() const;
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getTileTextureOffset(fe::guid tileID) const;
                    FLAT_ENGINE_API const imp::tile *getTile(fe::guid tileID) const;

                    // Create a tile and put it into the fabrications
                    FLAT_ENGINE_API void create(fe::guid name, fe::Vector2<unsigned int> size, fe::Vector2<unsigned int> offset);

                    FLAT_ENGINE_API fe::Handle add(fe::Vector2d position, fe::guid tileId);
                    FLAT_ENGINE_API void remove(fe::Handle handle);
                    FLAT_ENGINE_API fe::Handle get(fe::Vector2d position);

                    FLAT_ENGINE_API void draw(sf::RenderTarget &target, sf::RenderStates states);

                    // Serializes all tiles to a file. Does not serialize fabrications
                    FLAT_ENGINE_API void serialize(fe::serializerID &serial);
                    // Deserializes all tiles. Does not deserialize fabrications
                    FLAT_ENGINE_API void deserialize(fe::serializerID &serial);

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