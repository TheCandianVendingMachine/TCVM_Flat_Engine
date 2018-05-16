// gameState.hpp
// a base class for all game states that exist
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/memoryManager.hpp"
#include "../../debug/logger.hpp"
#include "../graphic/camera.hpp"
#include "gameWorld.hpp"
#include "../entitySpawner/entitySpawner.hpp"

#include <vector>
#include <queue>

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
                class panel;
                class guiElement;
            }

        struct prefabObject;

        class baseGameState
            {
                private:
                    std::vector<gui::panel*> m_guiPanels;
                    std::queue<gui::panel*> m_guiPanelsToAdd;
                    fe::gameWorld m_gameWorld;
                    fe::entitySpawner m_entitySpawner;
                    fe::camera m_stateCamera;

                    bool m_paused; // If state is paused no updates will occur

                protected:
                    virtual void drawExtra(sf::RenderTarget &app) {}

                public:
                    FLAT_ENGINE_API baseGameState() : m_gameWorld(this), m_paused(false) {}

                    FLAT_ENGINE_API void addPanel(gui::panel *panel);
                    FLAT_ENGINE_API void removePanel(fe::str panelID);
                    FLAT_ENGINE_API void removePanel(gui::panel *panel);
                    FLAT_ENGINE_API fe::gui::panel *getPanel(fe::str panelID);
                    // Run a function to check every single panel
                    template<typename Func>
                    bool forEachPanel(Func func);

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

                    FLAT_ENGINE_API const fe::gameWorld &getGameWorld() const;
                    FLAT_ENGINE_API fe::gameWorld &getGameWorld();

                    FLAT_ENGINE_API void setCamera(const fe::camera &camera);
                    FLAT_ENGINE_API void setCamera(const fe::camera &&camera);
                    FLAT_ENGINE_API fe::camera &getCamera();

                    FLAT_ENGINE_API void pause(bool pause);
                    FLAT_ENGINE_API bool isPaused() const;

                    FLAT_ENGINE_API virtual ~baseGameState();
            };

        template<typename Func>
        bool baseGameState::forEachPanel(Func func)
            {
                for (auto &panel : m_guiPanels)
                    {
                        if (!func(panel))
                            {
                                return false;
                            }
                    }
                return true;
            }
    }