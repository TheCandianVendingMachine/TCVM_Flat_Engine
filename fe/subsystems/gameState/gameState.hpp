// gameState.hpp
// a base class for all game states that exist
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class gameState
            {
                private:
                public:
                    FLAT_ENGINE_EXPORT void preUpdate();
                    FLAT_ENGINE_EXPORT void update(float deltaTime);
                    FLAT_ENGINE_EXPORT void postUpdate();

                    FLAT_ENGINE_EXPORT void preDraw();
                    FLAT_ENGINE_EXPORT void draw(sf::RenderWindow &app);
                    FLAT_ENGINE_EXPORT void postDraw();
            };
    }