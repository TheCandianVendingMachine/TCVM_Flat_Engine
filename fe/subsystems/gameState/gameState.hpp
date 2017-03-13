// gameState.hpp
// a base class for all game states that exist
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../graphic/sceneGraph.hpp"

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class gameState
            {
                private:
                    fe::sceneGraph m_sceneGraph;

                public:
                    FLAT_ENGINE_API virtual void init();

                    FLAT_ENGINE_EXPORT virtual void preUpdate();
                    FLAT_ENGINE_EXPORT void update(float deltaTime);
                    FLAT_ENGINE_EXPORT virtual void postUpdate();

                    FLAT_ENGINE_EXPORT virtual void preDraw();
                    FLAT_ENGINE_EXPORT void draw(sf::RenderWindow &app);
                    FLAT_ENGINE_EXPORT virtual void postDraw();

                    FLAT_ENGINE_API virtual void deinit();

                    FLAT_ENGINE_API fe::sceneGraph *getSceneGraph();
            };
    }