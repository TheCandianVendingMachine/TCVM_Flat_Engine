// gameState.hpp
// a base class for all game states that exist
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../graphic/renderObject/sceneGraph.hpp"
#include "../../entity/baseEntity.hpp"
#include "../physics/collision/collisionHandler.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../memory/memoryManager.hpp"
#include "../../debug/logger.hpp"

#include <vector>

namespace sf
    {
        class RenderTarget;
        class Event;
    }

namespace fe
    {
        namespace gui
            {
                class panel;
                class guiElement;
            }

        class baseGameState : public fe::handleManager<baseEntity*, FE_MAX_GAME_OBJECTS>
            {
                private:
                    std::vector<gui::panel*> m_guiPanels;

                    fe::sceneGraph m_sceneGraph;
                    fe::collisonHandler m_collisionHandler;

                protected:
                    FLAT_ENGINE_API void addPanel(gui::panel *panel);
                    FLAT_ENGINE_API void removePanel(gui::panel *panel);

                    FLAT_ENGINE_API void onAdd(baseEntity *object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API baseGameState() {}

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API virtual void init() {}

                    FLAT_ENGINE_API virtual void onActive() {}
                    FLAT_ENGINE_API virtual void onDeactive() {}


                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API virtual void handleWindowEvent(const sf::Event &event) {}

                    FLAT_ENGINE_API virtual void preUpdate() {}
                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void postUpdateDefined();
                    FLAT_ENGINE_API virtual void postUpdate() {}

                    FLAT_ENGINE_API virtual void preDraw() {}
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);
                    FLAT_ENGINE_API virtual void postDraw() {}

                    FLAT_ENGINE_API virtual void deinit() {}
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API fe::sceneGraph &getSceneGraph();
                    FLAT_ENGINE_API fe::collisonHandler &getCollisionHandler();

                    FLAT_ENGINE_API virtual ~baseGameState();
            };

    }