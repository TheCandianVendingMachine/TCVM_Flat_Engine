#include "fe/localization/localizationStorage.hpp"
#include "fe/subsystems/parsing/xmlParsing.hpp"
#include "fe/objectManagement/str.hpp"
#include <rapidxml_print.hpp>
#include <iostream>

void fe::localizationStorage::load(rapidxml::xml_node<> *currentNode)
    {
        if (!currentNode) return;

        for (rapidxml::xml_node<> *nextNode = currentNode; nextNode; nextNode = nextNode->first_node())
            {
                std::string name = nextNode->name();
                if (name == "Package")
                    {
                        #if !COMPILE_FULL_STACK
                            for (rapidxml::xml_node<> *idNodes = nextNode->first_node(); idNodes; idNodes = idNodes->next_sibling())
                                {
                                    std::string name = idNodes->name();
                                    if (name == "strId")
                                        {
                                            std::string id = idNodes->first_attribute()->value();
                                            for (rapidxml::xml_node<> *sibling = idNodes->first_node(); sibling; sibling = sibling->next_sibling())
                                                {
                                                    std::string language = sibling->first_attribute()->value();
                                                    char *content = sibling->value(); // char because of memory allocation bug with std::string
                                                    m_text[FE_STR(nextNode->first_attribute()->value())][FE_STR(id.c_str())][FE_STR(language.c_str())] = content;
                                                }
                                        }
                                }
                        #else
                            loadPackage(nextNode, FE_STR(nextNode->first_attribute()->value()));
                        #endif
                    }
            }
    }

void fe::localizationStorage::loadPackage(rapidxml::xml_node<> *packageBaseNode, fe::str package)
    {
        for (rapidxml::xml_node<> *idNodes = packageBaseNode->first_node(); idNodes; idNodes = idNodes->next_sibling())
            {
                std::string name = idNodes->name();
                if (name == "strId")
                    {
                        #if !COMPILE_FULL_STACK
                            std::string id = idNodes->first_attribute()->value();
                            for (rapidxml::xml_node<> *sibling = idNodes->first_node(); sibling; sibling = sibling->next_sibling())
                                {
                                    std::string language = sibling->first_attribute()->value();
                                    char *content = sibling->value(); // char because of memory allocation bug with std::string
                                    m_text[package][FE_STR(id.c_str())][FE_STR(language.c_str())] = content;
                                }
                        #else
                            loadID(idNodes, package);
                        #endif
                    }
            }
    }

void fe::localizationStorage::loadID(rapidxml::xml_node<> *idNode, fe::str package)
    {
        std::string id = idNode->first_attribute()->value();
        for (rapidxml::xml_node<> *sibling = idNode->first_node(); sibling; sibling = sibling->next_sibling())
            {
                std::string language = sibling->first_attribute()->value();
                std::string content = sibling->value();
                m_text[package][FE_STR(id.c_str())][FE_STR(language.c_str())] = content;
            }
    }

void fe::localizationStorage::load(const char *textFilepath)
    {
        std::string content;
        auto xmlDoc = fe::loadFile(textFilepath, content);

        // Loop through all packages
        for (rapidxml::xml_node<> *node = xmlDoc->first_node()->first_node(); node; node = node->next_sibling())
            {
                load(node);
            }

        fe::freeDocument(xmlDoc);
    }

const std::string &fe::localizationStorage::get(fe::str packageID, fe::str textID, fe::str language)
    {
        return m_text[packageID][textID][language];
    }
