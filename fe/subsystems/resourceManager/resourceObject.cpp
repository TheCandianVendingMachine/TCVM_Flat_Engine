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
                m_resourceData = FE_ALLOC_STACK("ImageResourceData", sizeof(imageLoaded.getPixelsPtr()));
                memcpy(m_resourceData, imageLoaded.getPixelsPtr(), sizeof(m_resourceData));
            }

        void *memory = FE_ALLOC_STACK("ImageResourceFilepath", sizeof(filepath));
        m_filepath = static_cast<char*>(memory);
        strcpy(m_filepath, filepath);
    }
