// localizationStorage.hpp
// All text that is localized is stored here. Contains std::unordered_map with a string and a map containing languages/text
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"

#include "../typeDefines.hpp"
#include <string>
#include <unordered_map>

namespace fe
    {
        class localizationStorage
            {
                private:
                    std::unordered_map<fe::guid, std::unordered_map<fe::guid, std::string>> m_text;

                public:
                    FLAT_ENGINE_API void load(const char *textFilepath);
                    FLAT_ENGINE_API std::string get(fe::guid textID, fe::guid language);

            };
    }