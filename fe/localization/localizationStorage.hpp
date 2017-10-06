// localizationStorage.hpp
// All text that is localized is stored here. Contains std::unordered_map with a string and a map containing languages/text
#pragma once
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
                    void load(const char *textFilepath);
                    std::string get(fe::guid textID, fe::guid language);

            };
    }