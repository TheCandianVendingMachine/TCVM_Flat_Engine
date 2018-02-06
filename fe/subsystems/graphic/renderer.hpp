// renderer.hpp
// A wrapper for rendering content with sf::RenderWindow. Takes in a sf::Drawable and draws
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../math/Vector2.hpp"
#include "../serializer/serializerID.hpp"
#include "../serializer/serializable.hpp"

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

                    struct settings : fe::serializable
                        {
                            fe::Vector2<unsigned int> m_windowSize;
                            bool m_fullscreen;
                            bool m_borderless;

                            unsigned int m_fps;

                            FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                            FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
                        };

                    settings m_settings;

                    friend class serializerID;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void save() const;
                    FLAT_ENGINE_API void load();

                    FLAT_ENGINE_API void draw(const sf::Drawable &draw);

                    FLAT_ENGINE_API sf::RenderWindow &getRenderWindow() const;
					FLAT_ENGINE_API fe::Vector2<unsigned int> getWindowSize();

                    FLAT_ENGINE_API virtual ~renderer();
            };
    }