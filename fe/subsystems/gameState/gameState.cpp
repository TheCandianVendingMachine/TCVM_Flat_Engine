#include "gameState.hpp"
#include "../../gui/panel.hpp"
#include "../../gui/guiElement.hpp"

#include "../../debug/profiler.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

void fe::baseGameState::addPanel(gui::panel *panel)
    {
        m_guiPanels.push_back(panel);
    }

void fe::baseGameState::removePanel(gui::panel *panel)
    {
        m_guiPanels.erase(std::remove(m_guiPanels.begin(), m_guiPanels.end(), panel), m_guiPanels.end());
    }

void fe::baseGameState::onAdd(baseEntity *object, fe::Handle objectHandle)
    {
        object->onAdd(*this);
    }

void fe::baseGameState::startUp()
    {
        m_sceneGraph.startUp();
    }

void fe::baseGameState::handleEvents(const sf::Event &event)
    {
        for (auto &panel : m_guiPanels)
            {
                panel->handleEvent(event);
            }

        handleWindowEvent(event);
    }

void fe::baseGameState::update()
    {
        for (auto &ent : getObjects())
            {
                ent->update();
            }
    }

void fe::baseGameState::postUpdateDefined()
    {
        for (auto &ent : getObjects())
            {
                ent->postUpdate();
            }

        for (auto &panel : m_guiPanels)
            {
                panel->update();
            }
    }

void fe::baseGameState::draw(sf::RenderTarget &app)
    {
        FE_PROFILE("state_scene_graph")
        m_sceneGraph.draw(app);
        FE_END_PROFILE

        FE_PROFILE("state_gui")
        for (auto &panel : m_guiPanels)
            {
                panel->draw(app);
            }
        FE_END_PROFILE
    }

void fe::baseGameState::shutDown()
    {
        m_sceneGraph.shutDown();
    }

fe::sceneGraph &fe::baseGameState::getSceneGraph()
    {
        return m_sceneGraph;
    }

fe::collisonHandler &fe::baseGameState::getCollisionHandler()
    {
        return m_collisionHandler;
    }

fe::baseGameState::~baseGameState()
    {}

