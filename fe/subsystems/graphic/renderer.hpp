// renderer.hpp
// A wrapper for rendering content with sf::RenderWindow. Takes in a sf::Drawable and a fe::Transformable, modifies the position and draws it
#pragma once

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

                public:
                    void startUp();
                    void shutDown();

                    void load();

                    void draw(const sf::Drawable &draw);

                    sf::RenderWindow &getRenderWindow();
            };
    }