// gameState.hpp
// a base class for all game states that exist
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/memoryManager.hpp"
#include "../../debug/logger.hpp"
#include "gameWorld.hpp"

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

        class baseGameState
            {
                private:
                    std::vector<gui::panel*> m_guiPanels;
                    std::queue<gui::panel*> m_guiPanelsToAdd;
                    fe::gameWorld m_gameWorld;

                protected:
                    virtual void drawExtra(sf::RenderTarget &app) {}

                public:
                    FLAT_ENGINE_API baseGameState() {}

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
                    FLAT_ENGINE_API virtual void postUpdate() {}

                    FLAT_ENGINE_API void preDrawDefined();
                    FLAT_ENGINE_API virtual void preDraw() {}
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);
                    FLAT_ENGINE_API virtual void postDraw() {}

                    FLAT_ENGINE_API virtual void deinit() {}
                    FLAT_ENGINE_API void shutDown();

                    template<typename ...Args>
                    FLAT_ENGINE_API fe::Handle addObject(fe::baseEntity *ent, bool staticObject, Args &&...args);
                    FLAT_ENGINE_API void removeObject(fe::Handle ent);
                    FLAT_ENGINE_API void removeObject(fe::baseEntity *ent);
                    FLAT_ENGINE_API fe::baseEntity *getObject(fe::Handle handle) const;

                    FLAT_ENGINE_API const fe::gameWorld &getGameWorld() const;
                    FLAT_ENGINE_API fe::gameWorld &getGameWorld();

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

        template<typename ...Args>
        fe::Handle baseGameState::addObject(fe::baseEntity *ent, bool staticObject, Args &&...args)
            {
                fe::Handle objhandle = m_gameWorld.addGameObject(ent, std::forward<Args>(args)...);
                m_gameWorld.getObject(objhandle)->onAdd(*this);
                return objhandle;
            }

}