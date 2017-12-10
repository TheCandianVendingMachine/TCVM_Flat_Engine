#include "guiPrefabricatedElements.hpp"
#include "../subsystems/parsing/xmlParsing.hpp"
#include "../debug/logger.hpp"
#include "panel.hpp"
#include "guiElement.hpp"
#include "label.hpp"
#include "button.hpp"
#include "square.hpp"
#include "../engine.hpp"
#include "../subsystems/resourceManager/resourceManager.hpp"
#include <SFML/Graphics/Font.hpp>
#include <algorithm>

void fe::guiPrefabricatedElements::loadElements(const char *filepath)
    {
        std::string content = "";
        rapidxml::xml_document<> *loaded = fe::loadFile(filepath, content);
        rapidxml::xml_node<> *firstNode = loaded->first_node();

        while (firstNode)
            {
                std::string name = firstNode->name();
                if (name == "element")
                    {
                        priv::elementTemplate element;
                        rapidxml::xml_node<> *nodeIt = firstNode->first_node();
                        while (nodeIt)
                            {
                                element.load(nodeIt);
                                nodeIt = nodeIt->next_sibling();
                            }
                        m_prefabElements[FE_STR(firstNode->first_attribute()->value())] = element;
                    }
                else if (name == "panel")
                    {
                        priv::panelTemplate panel;
                        rapidxml::xml_node<> *nodeIt = firstNode->first_node();
                        while (nodeIt)
                            {
                                panel.load(nodeIt);
                                nodeIt = nodeIt->next_sibling();
                            }
                        m_prefabPanels[FE_STR(firstNode->first_attribute()->value())] = panel;
                    }
                firstNode = firstNode->next_sibling();
            }

        fe::freeDocument(loaded);
    }

void fe::guiPrefabricatedElements::loadGUI(const char *filepath)
    {
        std::string content = "";
        rapidxml::xml_document<> *loaded = fe::loadFile(filepath, content);
        rapidxml::xml_node<> *firstNode = loaded->first_node();

        while (firstNode)
            {
                std::string name = firstNode->name();
                if (name == "gui")
                    {
                        priv::guiTemplate gui;
                        rapidxml::xml_node<> *nodeIt = firstNode->first_node();
                        while (nodeIt)
                            {
                                gui.load(nodeIt);
                                nodeIt = nodeIt->next_sibling();
                            }
                        m_prefabGUI[FE_STR(firstNode->first_attribute()->value())] = gui;
                    }
                firstNode = firstNode->next_sibling();
            }

        fe::freeDocument(loaded);
    }

fe::gui::guiElement *fe::guiPrefabricatedElements::getElement(const char *elementPrefabId)
    {
        return getElement(FE_STR(elementPrefabId));
    }

fe::gui::guiElement *fe::guiPrefabricatedElements::getElement(fe::guid elementPrefabId)
    {
        fe::priv::elementTemplate &elementTemplate = m_prefabElements[elementPrefabId];
        fe::gui::guiElement *element = nullptr;
        fe::Vector2d size(elementTemplate.m_size.x, elementTemplate.m_size.y);

        if (elementTemplate.m_elementType == priv::elementTemplate::BUTTON)
            {
                element = new fe::gui::button(size, [](){});
            }
        else if (elementTemplate.m_elementType == priv::elementTemplate::LABEL)
            {
                element = new fe::gui::label(*fe::engine::get().getResourceManager<sf::Font>()->get(elementTemplate.m_label.fontID), elementTemplate.m_label.defaultText);
            }
        else if (elementTemplate.m_elementType == priv::elementTemplate::SQUARE)
            {
                const sf::Texture *texture = fe::engine::get().getResourceManager<sf::Texture>()->getTexture(elementTemplate.m_texture.textureID);
                const fe::Vector2<unsigned int> offset = fe::engine::get().getResourceManager<sf::Texture>()->getTexturePosition(elementTemplate.m_texture.textureID);
                element = new fe::gui::square(texture, fe::Vector2d(offset.x, offset.y));
            }
        else if (elementTemplate.m_elementType == priv::elementTemplate::TEXT_BOX)
            {
                auto textAttr = elementTemplate.m_textBox.textAttr;
                element = new fe::gui::textBox( size, 
                                                *fe::engine::get().getResourceManager<sf::Font>()->get(textAttr.fontID),
                                                fe::gui::textBox::options(elementTemplate.m_textBox.options),
                                                elementTemplate.m_textBox.textStringSize,
                                                elementTemplate.m_textBox.maxCharacters,
                                                elementTemplate.m_textBox.padding.x,
                                                elementTemplate.m_textBox.padding.y,
                                                textAttr.defaultText);
            }
        else
            {
                element = new fe::gui::guiElement();
            }
        
        element->setSize(size);
        element->setInactiveColour(sf::Color(   elementTemplate.m_inactiveColour.r,
                                                elementTemplate.m_inactiveColour.g, 
                                                elementTemplate.m_inactiveColour.b, 
                                                elementTemplate.m_inactiveColour.a));
        element->setActiveColour(sf::Color( elementTemplate.m_activeColour.r,
                                            elementTemplate.m_activeColour.g, 
                                            elementTemplate.m_activeColour.b, 
                                            elementTemplate.m_activeColour.a));

        return element;
    }

fe::gui::panel *fe::guiPrefabricatedElements::getPanel(const char *panelPrefabId)
    {
        return getPanel(FE_STR(panelPrefabId));
    }

fe::gui::panel *fe::guiPrefabricatedElements::getPanel(fe::guid panelPrefabId)
    {
        fe::priv::panelTemplate &panelTemplate = m_prefabPanels[panelPrefabId];
        fe::gui::panel *panel = new fe::gui::panel( panelPrefabId,
                                                    fe::Vector2d(panelTemplate.m_size.x, panelTemplate.m_size.y),
                                                    panelTemplate.options,
                                                    panelTemplate.m_title.defaultText,
                                                    fe::engine::get().getResourceManager<sf::Font>()->get(panelTemplate.m_title.fontID));

        panel->setPanelBarColour(sf::Color( panelTemplate.m_titleBarColour.r,
                                            panelTemplate.m_titleBarColour.g,
                                            panelTemplate.m_titleBarColour.b,
                                            panelTemplate.m_titleBarColour.a));

        panel->setPanelClearColour(sf::Color(   panelTemplate.m_bodyColour.r,
                                                panelTemplate.m_bodyColour.g,
                                                panelTemplate.m_bodyColour.b,
                                                panelTemplate.m_bodyColour.a));

        return panel;
    }

fe::gui::panel *fe::guiPrefabricatedElements::getGUI(const char *guiPrefabId)
    {
        return getGUI(FE_STR(guiPrefabId));
    }

fe::gui::panel *fe::guiPrefabricatedElements::getGUI(fe::guid guiPrefabId)
    {
        fe::priv::guiTemplate &guiTemplate = m_prefabGUI[guiPrefabId];
        fe::gui::panel *panel = getPanel(guiTemplate.m_panel.id);

        panel->setPosition(guiTemplate.m_panel.position.x, guiTemplate.m_panel.position.y);
        panel->setEventOnClose(guiTemplate.m_panel.eventOnClose);
        panel->setEventOnMinimize(guiTemplate.m_panel.eventOnMinimize);

        for (auto &element : guiTemplate.m_elements)
            {
                fe::Handle elementId = panel->addElement(getElement(element.id));
                panel->setElementPosition(elementId, { element.position.x, element.position.y });
                panel->getElement(elementId)->setEvent(element.event);
            }

        return panel;
    }

void fe::priv::elementTemplate::load(rapidxml::xml_node<> *node)
    {
        std::string nodeName = node->name();
        if (nodeName == "type")
            {
                std::string type = node->value();
                if (type == "BUTTON")
                    {
                        m_elementType = BUTTON;
                    }
                else if (type == "LABEL")
                    {
                        m_elementType = LABEL;
                    }
                else if (type == "SQUARE")
                    {
                        m_elementType = SQUARE;
                    }
                else if (type == "TEXT_BOX")
                    {
                        m_elementType = TEXT_BOX;
                    }
            }
        else if (nodeName == "size")
            {
                m_size.load(node->first_node());
            }
        else if (nodeName == "texture")
            {
                m_texture.load(node->first_node());
            }
        else if (nodeName == "colour")
            {
                rapidxml::xml_node<> *nodeIt = node->first_node();
                while (nodeIt)
                    {
                        if (std::string(nodeIt->name()) == "active")
                            {
                                m_activeColour.load(nodeIt->first_node());
                            }
                        else if (std::string(nodeIt->name()) == "inactive")
                            {
                                m_inactiveColour.load(nodeIt->first_node());
                            }
                        nodeIt = nodeIt->next_sibling();
                    }
            }
        else if (nodeName == "label")
            {
                m_label.load(node->first_node());
            }
        else if (nodeName == "text_box")
            {
                m_textBox.load(node->first_node());
            }
        else
            {
                FE_LOG_WARNING(nodeName + "not supported by GUI prefab");
            }
    }

void fe::priv::text::load(rapidxml::xml_node<>* node)
    {
        rapidxml::xml_node<> *nodeIt = node;
        while (nodeIt)
            {
                if (std::string(nodeIt->name()) == "font")
                    {
                        fontID = FE_STR(nodeIt->value());
                    }
                else if (std::string(nodeIt->name()) == "default_text")
                    {
                        defaultText = std::string(nodeIt->value());
                    }
                nodeIt = nodeIt->next_sibling();
            }
    }

void fe::priv::colour::load(rapidxml::xml_node<>* node)
    {
        rapidxml::xml_node<> *nodeIt = node;
        while (nodeIt)
            {
                if (std::string(nodeIt->name()) == "r")
                    {
                        r = std::stof(nodeIt->value());
                    }
                else if (std::string(nodeIt->name()) == "g")
                    {
                        g = std::stof(nodeIt->value());
                    }
                else if (std::string(nodeIt->name()) == "b")
                    {
                        b = std::stof(nodeIt->value());
                    }
                else if (std::string(nodeIt->name()) == "a")
                    {
                        a = std::stof(nodeIt->value());
                    }
                nodeIt = nodeIt->next_sibling();
            }
    }

void fe::priv::coord::load(rapidxml::xml_node<>* node)
    {
        rapidxml::xml_node<> *nodeIt = node;
        while (nodeIt)
            {
                if (std::string(nodeIt->name()) == "x")
                    {
                        x = interpretConstant(nodeIt->value());
                    }
                else if (std::string(nodeIt->name()) == "y")
                    {
                        y = interpretConstant(nodeIt->value());
                    }
                nodeIt = nodeIt->next_sibling();
            }
    }

void fe::priv::panelTemplate::loadOptions(rapidxml::xml_node<>* node)
    {
        rapidxml::xml_node<> *nodeIt = node;
        options = 0;
        while (nodeIt)
            {
                if (std::string(nodeIt->name()) == "has_title")
                    {
                        m_title.load(nodeIt->first_node());
                        options |= fe::gui::panelModifiers::HAS_TITLE;
                    }
                else if (std::string(nodeIt->name()) == "can_drag" && priv::interpretConstant(nodeIt->value()))
                    {
                        options |= fe::gui::panelModifiers::CAN_DRAG;
                    }
                else if (std::string(nodeIt->name()) == "can_close" && priv::interpretConstant(nodeIt->value()))
                    {
                        options |= fe::gui::panelModifiers::CAN_CLOSE;
                    }
                else if (std::string(nodeIt->name()) == "can_minimize" && priv::interpretConstant(nodeIt->value()))
                    {
                        options |= fe::gui::panelModifiers::CAN_MINIMIZE;
                    }
                nodeIt = nodeIt->next_sibling();
            }
    }

void fe::priv::panelTemplate::load(rapidxml::xml_node<> *node)
    {
        std::string nodeName = std::string(node->name());
        if (nodeName == "size")
            {
                m_size.load(node->first_node());
            }
        else if (nodeName == "options")
            {
                loadOptions(node->first_node());
            }
        else if (nodeName == "colour")
            {
                rapidxml::xml_node<> *colourIt = node->first_node();
                while (colourIt)
                    {
                        if (std::string(colourIt->name()) == "title_bar")
                            {
                                m_titleBarColour.load(colourIt->first_node());
                            }
                        else if (std::string(colourIt->name()) == "body")
                            {
                                m_bodyColour.load(colourIt->first_node());
                            }
                        colourIt = colourIt->next_sibling();
                    }
            }
    }

void fe::priv::guiTemplate::load(rapidxml::xml_node<> *node)
    {
        std::string nodeName = std::string(node->name());
        if (nodeName == "panel")
            {
                m_panel.load(node->first_node());
            }
        else if (nodeName == "element")
            {
                guiTemplate::element element;
                element.load(node->first_node());
                m_elements.push_back(element);
            }
    }

float fe::priv::interpretConstant(const std::string &constant)
    {
        if (constant.empty()) return 0;
        bool isAlpha = false;
        std::for_each(constant.begin(), constant.end(), [&isAlpha](char c) { isAlpha |= std::isalpha(c); });
        if (!isAlpha)
            {
                return std::stof(constant);
            }

        std::string constantCheck = constant;
        std::transform(constantCheck.begin(), constantCheck.end(), constantCheck.begin(), [](char c) { return std::toupper(c); });

        if (constantCheck == "SCREEN_SIZE_X")
            {
                return fe::engine::get().getWindowSize().x;
            }
        else if (constantCheck == "SCREEN_SIZE_Y")
            {
                return fe::engine::get().getWindowSize().y;
            }
        else if (constantCheck == "TRUE")
            {
                return 1;
            }
        else if (constantCheck == "FALSE")
            {
                return 0;
            }

        return 0;
    }
