// gameState.hpp
// a base class for all game states that exist
#pragma once

#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/memory/memoryManager.hpp"
#include "fe/debug/logger.hpp"
#include "fe/subsystems/graphic/camera.hpp"
#include "gameWorld.hpp"
#include "fe/subsystems/entitySpawner/entitySpawner.hpp"
#include "fe/gui/guiGraph.hpp"
#include "gameScreen.hpp"

#include "fe/subsystems/particle/particleSystem.hpp"

#include <vector>
#include <queue>
#include <memory>

namespace sf
    {
        class RenderTarget;
        class Event;
    }

namespace fe
    {
        class collisionWorld;
        class baseEntity;
        namespace gui
            {
                class dialog;
            }

        struct prefabObject;

        class baseGameState
            {
                private:
                    fe::baseGameScreen *m_nextGameScreen;
                    bool m_newScreenAvaliable;
                    std::unique_ptr<fe::baseGameScreen> m_currentScreen;

                    std::vector<gui::dialog*> m_dialogs;
                    fe::gui::guiGraph m_guiGraph;
                    fe::gameWorld m_gameWorld;
                    fe::entitySpawner m_entitySpawner;
                    fe::camera m_stateCamera;
                    fe::particleSystem m_particleSystem;

                    bool m_paused; // If state is paused no updates will occur

                    friend class entityWorld;

                protected:
                    virtual void drawExtra(sf::RenderTarget &app) {}

                public:
                    FLAT_ENGINE_API baseGameState();

                    FLAT_ENGINE_API int addDialog(fe::gui::dialog *panel, int connected = -1, int zPos = 0);
                    FLAT_ENGINE_API void removeDialog(fe::gui::dialog *panel);

                    FLAT_ENGINE_API void changeScreen(fe::baseGameScreen *newScreen);
                    FLAT_ENGINE_API void popScreen();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API virtual void init() {}

                    FLAT_ENGINE_API virtual void onActive() {}
                    FLAT_ENGINE_API virtual void onDeactive() {}


                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API virtual void handleWindowEvent(const sf::Event &event) {}

                    FLAT_ENGINE_API void preUpdateDefined();
                    FLAT_ENGINE_API virtual void preUpdate() {}
                    FLAT_ENGINE_API virtual void update() {}
                    FLAT_ENGINE_API void updateDefined(collisionWorld *collisionWorld);
                    FLAT_ENGINE_API void postUpdateDefined();
                    FLAT_ENGINE_API virtual void fixedUpdate(float deltaTime) {}
                    FLAT_ENGINE_API void fixedUpdateDefined(float deltaTime);
                    FLAT_ENGINE_API void updateCamera(float deltaTime, int iterations);
                    FLAT_ENGINE_API virtual void postUpdate() {}

                    FLAT_ENGINE_API void preDrawDefined();
                    FLAT_ENGINE_API virtual void preDraw() {}
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);
                    FLAT_ENGINE_API virtual void postDraw() {}

                    FLAT_ENGINE_API virtual void deinit() {}
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API fe::Handle addObject(const char *id);
                    FLAT_ENGINE_API void removeObject(fe::Handle ent);
                    FLAT_ENGINE_API void removeObject(fe::baseEntity *ent);
                    FLAT_ENGINE_API fe::baseEntity *getObject(fe::Handle handle) const;
                    FLAT_ENGINE_API void getAllObjects(std::vector<fe::baseEntity*> &entities);
                    FLAT_ENGINE_API fe::prefabObject &addPrefab(const char *name);
                    FLAT_ENGINE_API void getPrefabs(std::vector<std::string> &prefabs) const;

                    FLAT_ENGINE_API const fe::particleSystem &getParticleSystem() const;
                    FLAT_ENGINE_API fe::particleSystem &getParticleSystem();

                    FLAT_ENGINE_API const fe::gameWorld &getGameWorld() const;
                    FLAT_ENGINE_API fe::gameWorld &getGameWorld();

                    FLAT_ENGINE_API void setCamera(const fe::camera &camera);
                    FLAT_ENGINE_API void setCamera(const fe::camera &&camera);
                    FLAT_ENGINE_API fe::camera &getCamera();

                    FLAT_ENGINE_API void pause(bool pause);
                    FLAT_ENGINE_API bool isPaused() const;

                    FLAT_ENGINE_API virtual ~baseGameState();
            };
    }