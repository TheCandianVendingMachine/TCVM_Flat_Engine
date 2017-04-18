// gameState.hpp
// a base class for all game states that exist
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../graphic/sceneGraph.hpp"
#include "../../entity/baseEntity.hpp"
#include "../collision/collisionHandler.hpp"
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

        class baseGameState
            {
                private:
                    std::vector<gui::panel*> m_guiPanels;

                    fe::sceneGraph m_sceneGraph;
                    fe::collisonHandler m_collisionHandler;

                protected:
                    template<typename T, typename ...Args>
                    FLAT_ENGINE_API fe::sceneGraph::EntityHandle addEntity(Args &&...args);
                    FLAT_ENGINE_API void removeEntity(fe::sceneGraph::EntityHandle handle);

                    FLAT_ENGINE_API void addPanel(gui::panel *panel);
                    FLAT_ENGINE_API void removePanel(gui::panel *panel);

                public:
                    FLAT_ENGINE_API virtual void init() {}

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);

                    FLAT_ENGINE_API virtual void preUpdate() {}
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdateDefined();
                    FLAT_ENGINE_API virtual void postUpdate() {}

                    FLAT_ENGINE_API virtual void preDraw() {}
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);
                    FLAT_ENGINE_API virtual void postDraw() {}

                    FLAT_ENGINE_API virtual void deinit() {}

                    FLAT_ENGINE_API fe::sceneGraph &getSceneGraph();
                    FLAT_ENGINE_API fe::collisonHandler &getCollisionHandler();

                    template<typename T = fe::baseEntity>
                    FLAT_ENGINE_API T *getEntity(fe::sceneGraph::EntityHandle handle) const;

                    FLAT_ENGINE_API virtual ~baseGameState() {}
            };

        template<typename T, typename ...Args>
        fe::sceneGraph::EntityHandle baseGameState::addEntity(Args &&...args)
            {
                auto mem = FE_ALLOC_STACK("GameStateEntity", sizeof(T));
                if (mem) 
                    {
                        T *ent = new(mem) T(args...);
                        auto handle = m_sceneGraph.addEntity(ent);
                        ent->onAdd(*this);
                        return handle;
                    }
                else
                    {
                        FE_LOG_ERROR("Cannot add entity");
                    }

                return 0;
            }

        template<typename T>
        T *baseGameState::getEntity(fe::sceneGraph::EntityHandle handle) const
            {
            return static_cast<T*>(m_sceneGraph.getEntity(handle));
            }
    }