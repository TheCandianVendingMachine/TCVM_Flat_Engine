// guiPrefabricatedElements.hpp
// prefabricated GUI elements. Reads from an XML file and stores the GUI's inside
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../objectManagement/str.hpp"
#include "textBox.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <rapidxml.hpp>
#include <cstring>

namespace fe
    {
        namespace priv
            {
                FLAT_ENGINE_API float interpretConstant(const std::string &constant);

                struct coord
                    {
                        float x = 0;
                        float y = 0;

                        FLAT_ENGINE_API void load(rapidxml::xml_node<> *node);
                    };

                struct colour
                    {
                        fe::uInt8 r = 200;   // default panel colour
                        fe::uInt8 g = 150;
                        fe::uInt8 b = 255;
                        fe::uInt8 a = 240;
                        FLAT_ENGINE_API void load(rapidxml::xml_node<> *node);
                    };

                struct text
                    {
                        std::string defaultText;
                        fe::str fontID;
                        unsigned int size = 32;
                        FLAT_ENGINE_API void load(rapidxml::xml_node<> *node);
                    };

                struct elementTemplate
                    {
                        coord m_size;
                        fe::str m_id;

                        struct
                            {
                                fe::str textureID;
                                coord m_size;
                                void load(rapidxml::xml_node<> *node)
                                    {
                                        rapidxml::xml_node<> *nodeIt = node;
                                        while (nodeIt)
                                            {
                                                if (std::string(nodeIt->name()) == "textureID")
                                                    {
                                                        textureID = FE_STR(node->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "size")
                                                    {
                                                        m_size.load(nodeIt->first_node());
                                                    }
                                                nodeIt = nodeIt->next_sibling();
                                            }
                                    }
                            } m_texture;

                        colour m_inactiveColour;
                        colour m_activeColour;

                        enum
                            {
                                BUTTON,
                                TOGGLE_BUTTON,
                                LABEL,
                                SQUARE,
                                TEXT_BOX
                            } m_elementType;

                        text m_label;

                        struct
                            {
                                text textAttr;
                                coord padding;
                                unsigned int textStringSize;
                                unsigned int maxCharacters;
                                short options;
                                void load(rapidxml::xml_node<> *node)
                                    {
                                        rapidxml::xml_node<> *nodeIt = node;
                                        while (nodeIt)
                                            {
                                                if (std::string(nodeIt->name()) == "text_attributes")
                                                    {
                                                        textAttr.load(nodeIt->first_node());
                                                    }
                                                else if (std::string(nodeIt->name()) == "options")
                                                    {
                                                        rapidxml::xml_node<> *optionIt = nodeIt->first_node();
                                                        options = 0;
                                                        while (optionIt)
                                                            {
                                                                if (std::string(optionIt->name()) == "enable_alphabet")
                                                                    {
                                                                        options |= (std::stoi(optionIt->value()) == 0) ? fe::gui::textBox::DISABLE_ALPHABET : 0;
                                                                    }
                                                                else if (std::string(optionIt->name()) == "enable_numerics")
                                                                    {
                                                                        options |= (std::stoi(optionIt->value()) == 0) ? fe::gui::textBox::DISABLE_NUMERICS : 0;
                                                                    }
                                                                else if (std::string(optionIt->name()) == "enable_input")
                                                                    {
                                                                        options |= (std::stoi(optionIt->value()) == 0) ? fe::gui::textBox::DISABLE_INPUT: 0;
                                                                    }
                                                                else if (std::string(optionIt->name()) == "enable_word_wrap")
                                                                    {
                                                                        options |= (std::stoi(optionIt->value()) != 0) ? fe::gui::textBox::WORD_WRAP : 0;
                                                                    }
                                                                else if (std::string(optionIt->name()) == "size_to_text")
                                                                    {
                                                                        options |= (std::stoi(optionIt->value()) != 0) ? fe::gui::textBox::SIZE_TO_TEXT : 0;
                                                                    }
                                                                else if (std::string(optionIt->name()) == "delete_initial_text_on_input")
                                                                    {
                                                                        options |= (std::stoi(optionIt->value()) != 0) ? fe::gui::textBox::DELETE_INITIAL_TEXT_ON_INPUT : 0;
                                                                    }
                                                                optionIt = optionIt->next_sibling();
                                                            }
                                                    }
                                                else if (std::string(nodeIt->name()) == "text_string_size")
                                                    {
                                                        textStringSize = std::stoi(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "max_characters")
                                                    {
                                                        maxCharacters = std::stoi(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "padding")
                                                    {
                                                        padding.load(nodeIt->first_node());
                                                    }
                                                nodeIt = nodeIt->next_sibling();
                                            }
                                    }
                            } m_textBox;

                        FLAT_ENGINE_API void load(rapidxml::xml_node<> *node);
                    };

                struct panelTemplate
                    {
                        coord m_size;

                        text m_title;
                        colour m_titleBarColour;
                        colour m_bodyColour;

                        short options;

                        FLAT_ENGINE_API void loadOptions(rapidxml::xml_node<> *node);
                        FLAT_ENGINE_API void load(rapidxml::xml_node<> *node);
                    };

                struct guiTemplate
                    {
                        struct 
                            {
                                coord position;
                                fe::str id; // id of the panel that this pulls from
                                fe::str eventOnClose = -1;
                                fe::str eventOnMinimize = -1;
                                void load(rapidxml::xml_node<> *node)
                                    {
                                        rapidxml::xml_node<> *nodeIt = node;
                                        while (nodeIt)
                                            {
                                                if (std::string(nodeIt->name()) == "id")
                                                    {
                                                        id = FE_STR(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "position")
                                                    {
                                                        position.load(nodeIt->first_node());
                                                    }
                                                else if (std::string(nodeIt->name()) == "event_on_close")
                                                    {
                                                        eventOnClose = FE_STR(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "event_on_minimize")
                                                    {
                                                        eventOnMinimize = FE_STR(nodeIt->value());
                                                    }
                                                nodeIt = nodeIt->next_sibling();
                                            }
                                    }
                            } m_panel;

                        struct element
                            {
                                std::vector<element> attachedElements;
                                coord position;
                                fe::str id; // id of the element that this pulls from
                                fe::str event = -1;
                                fe::str extraEvent = -1;
                                bool fitToParent = false;
                                void load(rapidxml::xml_node<> *node)
                                    {
                                        rapidxml::xml_node<> *nodeIt = node;
                                        while (nodeIt)
                                            {
                                                if (std::string(nodeIt->name()) == "id")
                                                    {
                                                        id = FE_STR(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "position")
                                                    {
                                                        position.load(nodeIt->first_node());
                                                    }
                                                else if (std::string(nodeIt->name()) == "event")
                                                    {
                                                        event = FE_STR(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "extra_event")
                                                    {
                                                        extraEvent = FE_STR(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "fit_to_parent")
                                                    {
                                                        fitToParent = (bool)interpretConstant(nodeIt->value());
                                                    }
                                                else if (std::string(nodeIt->name()) == "attached_elements")
                                                    {
                                                        rapidxml::xml_node<> *attachedNodeIt = nodeIt->first_node();
                                                        while (attachedNodeIt)
                                                            {
                                                                attachedElements.emplace_back();
                                                                attachedElements.back().load(attachedNodeIt->first_node());
                                                                attachedNodeIt = attachedNodeIt->next_sibling();
                                                            }
                                                    }
                                                nodeIt = nodeIt->next_sibling();
                                            }
                                    }
                            };
                        std::vector<element> m_elements;

                        FLAT_ENGINE_API void load(rapidxml::xml_node<> *node);
                    };
            }

        namespace gui
            {
                class panel;
                class guiElement;
            }

        class guiPrefabricatedElements
            {
                private:
                    std::unordered_map<fe::str, priv::elementTemplate> m_prefabElements;
                    std::unordered_map<fe::str, priv::panelTemplate> m_prefabPanels;
                    std::unordered_map<fe::str, priv::guiTemplate> m_prefabGUI;

                public:
                    FLAT_ENGINE_API void loadElements(const char *filepath);
                    FLAT_ENGINE_API void loadGUI(const char *filepath);

                    FLAT_ENGINE_API gui::guiElement *getElement(const char *elementPrefabId);
                    FLAT_ENGINE_API gui::guiElement *getElement(fe::str elementPrefabId);

                    FLAT_ENGINE_API gui::panel *getPanel(const char *panelPrefabId);
                    FLAT_ENGINE_API gui::panel *getPanel(fe::str panelPrefabId);

                    FLAT_ENGINE_API gui::panel *getGUI(const char *guiPrefabId);
                    FLAT_ENGINE_API gui::panel *getGUI(fe::str guiPrefabId);

            };
    }