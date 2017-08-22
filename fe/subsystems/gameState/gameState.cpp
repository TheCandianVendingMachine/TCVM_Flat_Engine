#include "gameState.hpp"
#include "../../gui/panel.hpp"
#include "../../gui/guiElement.hpp"
#include "../physics/collision/collisionWorld.hpp"

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
        object->initialize();
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

void fe::baseGameState::preUpdateDefined()
    {
        preUpdate();
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i])
                    {
                        objects[i]->updateModules();
                    }
            }
    }

void fe::baseGameState::update(collisionWorld *collisionWorld)
    {
        FE_ENGINE_PROFILE("game_state", "entity_update");
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i])
                    {
                        objects[i]->update();

                        if (objects[i]->m_moved)
                            {
                                collisionWorld->updateCollider(objects[i]->getCollider());
                            }
                    }
            }
        FE_END_PROFILE;
    }

void fe::baseGameState::postUpdateDefined()
    {
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i])
                    {
                        objects[i]->postUpdate();
                    }
            }

        for (auto &panel : m_guiPanels)
            {
                panel->update();
            }
    }

void fe::baseGameState::draw(sf::RenderTarget &app)
    {
        FE_ENGINE_PROFILE("state_scene", "graph_buf1_draw")
        m_sceneGraph.draw(app);
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
        m_sceneGraph.shutDown();
    }

fe::sceneGraph &fe::baseGameState::getSceneGraph()
    {
        return m_sceneGraph;
    }

fe::baseGameState::~baseGameState()
    {}

