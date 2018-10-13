// animationTexture.hpp
// an object that holds data relavent to animation textures
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/typeDefines.hpp"
#include "animationFrame.hpp"
#include <unordered_map>
#include <vector>
#include <utility>

namespace fe
    {
        class animationTexture
            {
                private:
                    fe::Vector2<unsigned int> m_frameSize;
                    fe::Vector2<unsigned int> m_framePosition;
                    fe::Vector2<unsigned int> m_animationSize;
                    fe::Vector2<unsigned int> m_totalFrames;

                    std::unordered_map<fe::str, std::vector<fe::animationFrame>> m_animationSequences;

                public:
                    FLAT_ENGINE_API animationTexture() {}
                    FLAT_ENGINE_API animationTexture(const fe::Vector2<unsigned int> frameSize, const fe::Vector2<unsigned int> frameOffset, const fe::Vector2<unsigned int> animationSize);
                    FLAT_ENGINE_API std::pair<fe::Vector2<unsigned int>, fe::Vector2<unsigned int>> getTexture(unsigned int x, unsigned int y);

                    FLAT_ENGINE_API void createAnimationSequence(fe::str sequence, std::vector<fe::animationFrame> &frames);
                    FLAT_ENGINE_API const std::vector<fe::animationFrame> &getAnimationSequence(fe::str str) const;

            };
    }