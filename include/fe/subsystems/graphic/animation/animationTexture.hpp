// animationTexture.hpp
// an object that holds data relavent to animation textures
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../math/Vector2.hpp"
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
                    fe::Vector2<unsigned int> m_frameSize;
                    fe::Vector2<unsigned int> m_framePosition;
                    fe::Vector2<unsigned int> m_animationSize;

                    unsigned int m_maxFrames;

                    // If the strip is vertical or not
                    bool m_vertical;

                public:
                    FLAT_ENGINE_API animationTexture() {}
                    FLAT_ENGINE_API animationTexture(const fe::Vector2<unsigned int> frameSize, const fe::Vector2<unsigned int> frameOffset, const fe::Vector2<unsigned int> animationSize, bool verticalStrip);
                    FLAT_ENGINE_API std::pair<fe::Vector2<unsigned int>, fe::Vector2<unsigned int>> getTexture(animationActor *actor);

            };
    }