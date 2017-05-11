// animationTexture.hpp
// an object that holds data relavent to animation textures
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>

namespace sf
    {
        class Texture;
    }

namespace fe
    {
        class animationActor;
        class animationTexture
            {
                private:
                    sf::Vector2u m_frameSize;
                    sf::Vector2u m_framePosition;
                    sf::Vector2u m_animationSize;

                    unsigned int m_maxFrames;

                    // If the strip is vertical or not
                    bool m_vertical;

                public:
                    FLAT_ENGINE_API animationTexture() {}
                    FLAT_ENGINE_API animationTexture(const sf::Vector2u frameSize, const sf::Vector2u frameOffset, const sf::Vector2u animationSize, bool verticalStrip);
                    FLAT_ENGINE_API std::pair<sf::Vector2u, sf::Vector2u> getTexture(animationActor *actor);

            };
    }