inline fe::resourceManager<sf::Texture>::resourceManager()
    {
        m_packed.createTexture();
    }

inline sf::Texture *fe::resourceManager<sf::Texture>::load(const char* filepath, const char* id)
    {
        sf::Texture *resource = getTexture(id);
        if (resource)
            {
                return resource;
            }

        sf::Texture *added = new sf::Texture();
        if (added)
            {
                added->loadFromFile(filepath);

                m_textures.push_back(added);
                m_packed.addTexture(*added, id);
                return added;
            }
        else
            {
                FE_LOG_WARNING("Texture\"", id, "\"from filepath\"", filepath, "\"cannot be loaded into memory");
            }

        return nullptr;
    }

inline sf::Texture *fe::resourceManager<sf::Texture>::add(sf::Texture *texture, const char *id)
    {
        m_textures.push_back(texture);
        m_packed.addTexture(*texture, id);
        return texture;
    }

inline fe::fontData fe::resourceManager<sf::Texture>::addFont(const sf::Font *font, const char *id, unsigned int size)
    {
        sf::Texture textures[FE_CHAR_END - FE_CHAR_START];
        fe::fontData positions;

        fe::texturePacker fontPacker({ 512, 512 });
        for (unsigned int i = FE_CHAR_START; i < FE_CHAR_END; i++)
            {
                sf::Glyph glyph = font->getGlyph(i, size, false);
                sf::Texture bmp = font->getTexture(size);

                sf::Image bitmap;
                bitmap.create(glyph.bounds.width, glyph.bounds.height);
                bitmap.copy(bmp.copyToImage(), 0, 0, glyph.textureRect);

                textures[i - FE_CHAR_START].loadFromImage(bitmap);

                char id[1] = { fe::uInt8(i) };
                positions.m_positions[i - FE_CHAR_START] = fontPacker.addTexture(textures[i - FE_CHAR_START], id);
            }

        sf::Texture *added = new sf::Texture();
        *added = fontPacker.getTexture();
        positions.m_texture = add(added, id);
        positions.m_font = font;
        positions.m_charSize = size;
        return positions;
    }

inline sf::Texture *fe::resourceManager<sf::Texture>::getTexture(const char* id)
    {
        return m_packed.getTexture(id);
    }

inline sf::Texture *fe::resourceManager<sf::Texture>::getTexture(fe::str id)
    {
        return m_packed.getTexture(id);
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTextureOffset(const char* id)
    {
        return m_packed.getTexturePosition(id);
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTextureOffset(fe::str id)
    {
        return m_packed.getTexturePosition(id);
    }

inline sf::Texture &fe::resourceManager<sf::Texture>::get()
    {
        return m_packed.getTexture();
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTexturePosition(const char* id)
    {
        return m_packed.getTexturePosition(id);
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTexturePosition(fe::str id)
    {
        return m_packed.getTexturePosition(id);
    }

inline void fe::resourceManager<sf::Texture>::shutDown()
    {}