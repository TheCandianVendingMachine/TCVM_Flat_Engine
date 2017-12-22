#include "gameState.hpp"
#include "../../gui/panel.hpp"
#include "../../gui/guiElement.hpp"
#include "../../debug/profiler.hpp"
#include "../../entity/baseEntity.hpp"
#include <algorithm>

void fe::baseGameState::addPanel(gui::panel *panel)
    {
        m_guiPanelsToAdd.push(panel);
    }

void fe::baseGameState::removePanel(fe::str panelID)
    {
        m_guiPanels.erase(std::remove_if(m_guiPanels.begin(), m_guiPanels.end(), [&panelID](fe::gui::panel *panelPtr) { return panelPtr->id() == panelID; }), m_guiPanels.end());
    }

void fe::baseGameState::removePanel(gui::panel *panel)
    {
        m_guiPanels.erase(std::remove(m_guiPanels.begin(), m_guiPanels.end(), panel), m_guiPanels.end());
    }

fe::gui::panel *fe::baseGameState::getPanel(fe::str panelID)
    {
        for (auto &panel : m_guiPanels)
            {
                if (panel->id() == panelID)
                    {
                        return panel;
                    }
            }
        return nullptr;
    }

void fe::baseGameState::startUp()
    {
        m_gameWorld.startUp();
    }

void fe::baseGameState::handleEvents(const sf::Event &event)
    {
        for (auto &panel : m_guiPanels)
            {
                panel->handleEvent(event);
            }

        handleWindowEvent(event);
    }

void fe::baseGameState::preUpdateDefined()
    {
        preUpdate();
        m_gameWorld.preUpdate();
    }

void fe::baseGameState::updateDefined(collisionWorld *collisionWorld)
    {
        update();
        FE_PROFILE("game_state", "game_world_update");
        m_gameWorld.update(collisionWorld);
        FE_END_PROFILE;
    }

void fe::baseGameState::postUpdateDefined()
    {
        m_gameWorld.postUpdate();

        while (!m_guiPanelsToAdd.empty())
            {
                m_guiPanels.push_back(m_guiPanelsToAdd.front());
                m_guiPanelsToAdd.pop();
            }

        for (auto it = m_guiPanels.begin(); it != m_guiPanels.end();)
            {
                if ((*it)->isKilled())
                    {
                        delete *it;
                        it = m_guiPanels.erase(it);
                    }
                else 
                    {
                        (*it)->update();
                        it++;
                    }
            }
    }

void fe::baseGameState::preDrawDefined()
    {
        m_gameWorld.preDraw();
    }

void fe::baseGameState::draw(sf::RenderTarget &app)
    {
        FE_ENGINE_PROFILE("state_scene", "graph_buf1_draw")
        m_gameWorld.draw(app);
        FE_END_PROFILE

        FE_ENGINE_PROFILE("state_gui", "buf1_draw")
        for (auto &panel : m_guiPanels)
            {
                panel->draw(app);
            }
        FE_END_PROFILE
    }

void fe::baseGameState::shutDown()
    {
        m_gameWorld.shutDown();
    }

void fe::baseGameState::removeObject(fe::Handle ent)
    {
        m_gameWorld.getObject(ent)->onRemove(*this);
        m_gameWorld.removeGameObject(ent);
    }

void fe::baseGameState::removeObject(fe::baseEntity *ent)
    {
        removeObject(ent->GUID());
    }

fe::baseEntity *fe::baseGameState::getObject(fe::Handle handle) const
    {
        return m_gameWorld.getObject(handle);
    }

const fe::gameWorld &fe::baseGameState::getGameWorld() const
    {
        return m_gameWorld;
    }

fe::gameWorld &fe::baseGameState::getGameWorld()
    {
        return m_gameWorld;
    }

fe::baseGameState::~baseGameState()
    {}

