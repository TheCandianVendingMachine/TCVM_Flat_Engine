#include "resourceObject.hpp"

#include <cstring>
#include <SFML/Graphics/Image.hpp>

#include "../../feAssert.hpp"
#include "../memory/memoryManager.hpp"

void fe::imageObject::load(const char *filepath)
    {
        sf::Image imageLoaded;
        if (!imageLoaded.loadFromFile(filepath))
            {
                FE_LOG_WARNING("\"", filepath, "\"", "not loaded correctly");
            }
        else
            {
                m_resourceData = FE_ALLOC_STACK(sizeof(imageLoaded.getPixelsPtr()));
                memcpy(m_resourceData, imageLoaded.getPixelsPtr(), sizeof(m_resourceData));
            }

        m_filepath = static_cast<char*>(FE_ALLOC_STACK(sizeof(filepath)));
        strcpy(m_filepath, filepath);
    }
