inline fe::resourceManager<sf::Texture>::resourceManager()
    {
        m_packed.createTexture();
    }

inline const sf::Texture *fe::resourceManager<sf::Texture>::load(const char* filepath, const char* id)
    {
        const sf::Texture *resource = getTexture(id);
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

inline const sf::Texture *fe::resourceManager<sf::Texture>::getTexture(const char* id)
    {
        return m_packed.getTexture(id);
    }

inline const sf::Texture *fe::resourceManager<sf::Texture>::getTexture(fe::guid id)
    {
        return m_packed.getTexture(id);
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTextureOffset(const char* id)
    {
        return m_packed.getTexturePosition(id);
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTextureOffset(fe::guid id)
    {
        return m_packed.getTexturePosition(id);
    }

inline const sf::Texture &fe::resourceManager<sf::Texture>::get()
    {
        return m_packed.getTexture();
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTexturePosition(const char* id)
    {
        return m_packed.getTexturePosition(id);
    }

inline fe::Vector2<unsigned int> fe::resourceManager<sf::Texture>::getTexturePosition(fe::guid id)
    {
        return m_packed.getTexturePosition(id);
    }

inline void fe::resourceManager<sf::Texture>::shutDown()
    {}