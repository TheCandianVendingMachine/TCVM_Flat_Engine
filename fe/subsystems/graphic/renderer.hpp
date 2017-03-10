// renderer.hpp
// A wrapper for rendering content with sf::RenderWindow. Takes in a sf::Drawable and a fe::Transformable, modifies the position and draws it
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

namespace sf
    {
        class RenderWindow;
        class Drawable;
    }

namespace fe
    {
        class renderer
            {
                private:
                    sf::RenderWindow *m_renderWindow;
                    static renderer *m_instance;

                public:
                    void startUp();
                    void shutDown();

                    FLAT_ENGINE_API static renderer &get();

                    void load();

                    FLAT_ENGINE_API void draw(const sf::Drawable &draw);

                    FLAT_ENGINE_API sf::RenderWindow &getRenderWindow();
            };
    }