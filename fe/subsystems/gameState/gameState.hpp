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

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class baseGameState
            {
                private:
                    fe::sceneGraph m_sceneGraph;
                    fe::collisonHandler m_collisionHandler;

                protected:
                    template<typename T, typename ...Args>
                    FLAT_ENGINE_API void addEntity(Args &&...args);
                    FLAT_ENGINE_API void removeEntity(baseEntity *entity);

                public:
                    FLAT_ENGINE_API virtual void init() {}

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

                    FLAT_ENGINE_API virtual ~baseGameState() {}
            };

        template<typename T, typename ...Args>
        void baseGameState::addEntity(Args &&...args)
            {
                auto mem = FE_ALLOC_STACK("GameStateEntity", sizeof(T));
                if (mem) 
                    {
                        T *ent = new(mem) T(args...);
                        m_sceneGraph.addEntity(ent);
                        ent->onAdd(*this);
                    }
                else
                    {
                        FE_LOG_ERROR("Cannot add entity");
                    }
            }
    }