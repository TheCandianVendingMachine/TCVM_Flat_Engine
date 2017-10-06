#include "localizationStorage.hpp"
#include "../subsystems/parsing/xmlParsing.hpp"

void fe::localizationStorage::load(const char *textFilepath)
    {
        auto xmlDoc = fe::loadFile(textFilepath);

        

        fe::freeDocument(xmlDoc);
    }

std::string fe::localizationStorage::get(fe::guid textID, fe::guid language)
    {
        return m_text[textID][language];
    }
