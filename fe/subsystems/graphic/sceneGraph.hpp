// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <vector>
#include <queue>

namespace sf
    {
        class Drawable;
    }

namespace fe
    {
        class sceneGraph
            {
                private:
                    std::vector<sf::Drawable*> m_drawables;
                    std::queue<sf::Drawable*> m_nextDrawable;

                public:
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void addDrawable(sf::Drawable *draw);
                    FLAT_ENGINE_API sf::Drawable *getNextDrawable();

            };
    }