// localizationStorage.hpp
// All text that is localized is stored here. Contains std::unordered_map with a string and a map containing languages/text
#pragma once
#define FLAT_ENGINE_EXPORT
#define COMPILE_FULL_STACK false

#include "../flatEngineExport.hpp"
#include "../typeDefines.hpp"
#include <string>
#include <unordered_map>

namespace rapidxml
    {
        template<class Ch = char>
        class xml_node;
    }

namespace fe
    {
        class localizationStorage
            {
                private:
                    // m_text[package][id][language] = text
                    std::unordered_map<fe::guid, std::unordered_map<fe::guid, std::unordered_map<fe::guid, std::string>>> m_text;

                    FLAT_ENGINE_API void load(rapidxml::xml_node<> *currentNode);
                    FLAT_ENGINE_API void loadPackage(rapidxml::xml_node<> *packageBaseNode, fe::guid package); // Not used if COMPILE_FULL_STACK is false
                    FLAT_ENGINE_API void loadID(rapidxml::xml_node<> *idNode, fe::guid package); // Not used if COMPILE_FULL_STACK is false

                public:
                    FLAT_ENGINE_API void load(const char *textFilepath);
                    FLAT_ENGINE_API const std::string &get(fe::guid packageID, fe::guid textID, fe::guid language);

            };
    }