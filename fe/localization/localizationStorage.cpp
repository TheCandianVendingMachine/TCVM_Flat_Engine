#include "localizationStorage.hpp"
#include "../subsystems/parsing/xmlParsing.hpp"
#include "../objectManagement/guid.hpp"
#include <rapidxml_print.hpp>
#include <iostream>

void fe::localizationStorage::load(const char *textFilepath)
    {
        std::string content;
        auto xmlDoc = fe::loadFile(textFilepath, content);

        std::string id = "";
        for (rapidxml::xml_node<> *node = xmlDoc->first_node(); node; node = node->first_node())
            {
                for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
                    {
                        std::string name = attr->name();
                        if (name == "ID")
                            {
                                id = attr->value();
                            }
                        else if (name == "language")
                            {
                                m_text[FE_STR(id.c_str())][FE_STR(attr->value())] = node->value();
                            }
                    }
            }

        fe::freeDocument(xmlDoc);
    }

std::string fe::localizationStorage::get(fe::guid textID, fe::guid language)
    {
        return m_text[textID][language];
    }
