#include "fe/subsystems/gameState/gameState.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/graphic/camera.hpp"
#include "fe/subsystems/gameState/gameWorld.hpp"
#include "fe/subsystems/collision/broadphaseAbstract.hpp"
#include "fe/gui/dialog.hpp"
#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>

fe::baseGameState::baseGameState() : 
    m_gameWorld(this),
    m_paused(false),
    m_nextGameScreen(nullptr),
    m_newScreenAvaliable(false)
    {
    }

int fe::baseGameState::addDialog(fe::gui::dialog *panel, int connected, int zPos)
    {
        m_dialogs.push_back(panel);
        return m_guiGraph.addObjectToGraph(panel, connected, zPos);
    }

void fe::baseGameState::removeDialog(fe::gui::dialog *panel)
    {
        panel->kill();
    }

void fe::baseGameState::startUp()
    {
        m_guiGraph.startUp();
        m_gameWorld.startUp();
        setCamera(std::move(fe::engine::get().getDefaultCamera()));
        m_entitySpawner.setWorld(&m_gameWorld);
    }

void fe::baseGameState::handleEvents(const sf::Event &event)
    {
        if (m_paused) return;
        handleWindowEvent(event);
        for (auto &dialog : m_dialogs)
            {
                dialog->handleEvent(event);
            }
    }

void fe::baseGameState::preUpdateDefined()
    {
        if (m_paused) return;
        if (m_newScreenAvaliable)
            {
                if (m_currentScreen)
                    {
                        m_currentScreen->deinit();
                    }

                m_currentScreen.reset(m_nextGameScreen);
                m_currentScreen->init();
                m_newScreenAvaliable = false;
            }

        preUpdate();

        if (m_currentScreen)
            {
                m_currentScreen->preUpdate();
            }

        m_gameWorld.preUpdate();
    }

void fe::baseGameState::updateDefined(collisionWorld *collisionWorld)
    {
        if (m_paused) return;

        update();
        FE_ENGINE_PROFILE("game_state", "game_world_update");
        m_gameWorld.update(collisionWorld);
        FE_END_PROFILE;
    }

void fe::baseGameState::postUpdateDefined()
    {
        if (m_paused) return;

        m_gameWorld.postUpdate();

        postUpdate();
        if (m_currentScreen)
            {
                m_currentScreen->preUpdate();
            }

        for (auto it = m_dialogs.begin(); it != m_dialogs.end();)
            {
                auto dialog = (*it);
                if (dialog->isKilled())
                    {
                        m_guiGraph.removeObjectFromGraph(dialog);
                        it = m_dialogs.erase(it);
                    }
                else
                    {
                        ++it;
                    }
            }
    }

void fe::baseGameState::fixedUpdateDefined(float deltaTime)
    {
        fixedUpdate(deltaTime);
        m_gameWorld.fixedUpdate(deltaTime);
    }

void fe::baseGameState::updateCamera(float deltaTime, int iterations)
    {
        if (m_paused) return;
        for (unsigned int i = 0; i < iterations; i++) 
            {
                m_stateCamera.updateCamera(deltaTime);
            }
    }

void fe::baseGameState::preDrawDefined()
    {
        m_gameWorld.preDraw();
        preDraw();
    }

void fe::baseGameState::draw(sf::RenderTarget &app)
    {
        FE_ENGINE_PROFILE("state_scene", "graph_buf1_draw")
        m_gameWorld.draw(app);
        FE_END_PROFILE

        FE_ENGINE_PROFILE("state_scene", "extra_draw")
        drawExtra(app);
        FE_END_PROFILE

        FE_ENGINE_PROFILE("state_gui", "buf1_draw");
        // Switch view to default engine view to allow drawing of gui in absolute terms. Near invisible for speed
        sf::View currentView = app.getView();
        //app.setView(fe::engine::get().getDefaultCamera().getView());
        m_guiGraph.draw(app);
        //app.setView(currentView);
        FE_END_PROFILE
    }

void fe::baseGameState::shutDown()
    {
        m_gameWorld.shutDown();
        m_guiGraph.shutDown();
    }

fe::Handle fe::baseGameState::addObject(const char *id)
    {
        fe::Handle entity = m_entitySpawner.spawn(id);
        getObject(entity)->enablePhysics(!isPaused());
        getObject(entity)->enableCollision(!isPaused());
        return entity;
    }

void fe::baseGameState::removeObject(fe::Handle ent)
    {
        removeObject(getObject(ent));
    }

void fe::baseGameState::removeObject(fe::baseEntity *ent)
    {
        ent->kill(true);
    }

fe::baseEntity *fe::baseGameState::getObject(fe::Handle handle) const
    {
        return m_gameWorld.getObject(handle);
    }

void fe::baseGameState::getAllObjects(std::vector<fe::baseEntity*> &entities)
    {
        m_gameWorld.getEntityWorld().getAllObjects(entities);
    }

fe::prefabObject &fe::baseGameState::addPrefab(const char *name)
    {
        return m_entitySpawner.createPrefab(name);
    }

void fe::baseGameState::getPrefabs(std::vector<std::string> &prefabs) const
    {
        m_entitySpawner.getPrefabs(prefabs);
    }

const fe::gameWorld &fe::baseGameState::getGameWorld() const
    {
        return m_gameWorld;
    }

fe::gameWorld &fe::baseGameState::getGameWorld()
    {
        return m_gameWorld;
    }

void fe::baseGameState::setCamera(const fe::camera &camera)
    {
        m_stateCamera = camera;
    }

void fe::baseGameState::setCamera(const fe::camera &&camera)
    {
        m_stateCamera = camera;
    }

fe::camera &fe::baseGameState::getCamera()
    {
        return m_stateCamera;
    }

void fe::baseGameState::pause(bool pause)
    {
        m_paused = pause;

        std::vector<fe::baseEntity*> objects;
        m_gameWorld.getEntityWorld().getAllObjects(objects);
        for (auto &obj : objects)
            {
                obj->enablePhysics(!pause);
                obj->enableCollision(!pause);
            }
    }

bool fe::baseGameState::isPaused() const
    {
        return m_paused;
    }

fe::baseGameState::~baseGameState()
    {}

