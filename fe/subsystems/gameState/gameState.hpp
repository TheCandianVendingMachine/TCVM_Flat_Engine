// gameState.hpp
// a base class for all game states that exist
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../graphic/sceneGraph.hpp"
#include "../../entity/baseEntity.hpp"

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class baseGameState
            {
                private:
                    fe::sceneGraph m_sceneGraph;

                protected:
                    template<typename T>
                    FLAT_ENGINE_API void addEntity();
                    FLAT_ENGINE_API void removeEntity(baseEntity *entity);

                public:
                    FLAT_ENGINE_API virtual void init() {}

                    FLAT_ENGINE_API virtual void preUpdate() {}
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API virtual void postUpdate() {}

                    FLAT_ENGINE_API virtual void preDraw() {}
                    FLAT_ENGINE_API void draw(sf::RenderWindow &app);
                    FLAT_ENGINE_API virtual void postDraw() {}

                    FLAT_ENGINE_API virtual void deinit() {}

                    FLAT_ENGINE_API fe::sceneGraph *getSceneGraph();

                    FLAT_ENGINE_API virtual ~baseGameState() {}
            };

        template<typename T>
        void baseGameState::addEntity()
            {
                auto mem = FE_ALLOC_STACK("GameStateEntity", sizeof(T));
                if (mem) 
                    {
                        T *ent = new(mem) T;
                        m_sceneGraph.addEntity(ent);
                    }
                else
                    {
                        FE_LOG_ERROR("Cannot add entity");
                    }
            }
    }