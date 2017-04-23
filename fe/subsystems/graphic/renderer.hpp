// renderer.hpp
// A wrapper for rendering content with sf::RenderWindow. Takes in a sf::Drawable and a fe::Transformable, modifies the position and draws it
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../math/Vector2.hpp"
#include "../serializer/serializerID.hpp"

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
                    fe::serializerID *m_windowSettings;

                    fe::Vector2<unsigned int> m_windowSize;
                    bool m_fullscreen;
                    bool m_borderless;

                    unsigned int m_fps;

                    SERIALIZE_ID("WindowSettings", "windowSizeX", m_windowSize.x, "windowSizeY", m_windowSize.y, "borderless", m_borderless, "fullscreen", m_fullscreen, "maxFPS", m_fps);

                    friend class serializerID;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void save();
                    FLAT_ENGINE_API void load();

                    FLAT_ENGINE_API void draw(const sf::Drawable &draw);

                    FLAT_ENGINE_API sf::RenderWindow &getRenderWindow();
					FLAT_ENGINE_API fe::Vector2d getWindowSize();

                    FLAT_ENGINE_API virtual ~renderer();
            };
    }