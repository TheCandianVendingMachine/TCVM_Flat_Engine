#include "fe/subsystems/graphic/animation/animationTexture.hpp"
#include "fe/subsystems/graphic/animation/animationActor.hpp"
#include "fe/debug/logger.hpp"
#include "fe/objectManagement/str.hpp"
#include "fe/engine.hpp"

fe::animationTexture::animationTexture(const fe::Vector2<unsigned int> frameSize, const fe::Vector2<unsigned int> frameOffset, const fe::Vector2<unsigned int> animationSize) :
    m_frameSize(frameSize),
    m_framePosition(frameOffset),
    m_animationSize(animationSize)
    {
        m_totalFrames.x = animationSize.x / frameSize.x;
        m_totalFrames.y = animationSize.y / frameSize.y;
    }

std::pair<fe::Vector2<unsigned int>, fe::Vector2<unsigned int>> fe::animationTexture::getTexture(unsigned int x, unsigned int y)
    {
        fe::Vector2<unsigned int> frameOffset = m_framePosition;
        frameOffset.x += (x * m_frameSize.x);
        frameOffset.y += (y * m_frameSize.y);

        return std::make_pair(frameOffset, m_frameSize);
    }

void fe::animationTexture::createAnimationSequence(fe::str sequence, std::vector<fe::animationFrame> &frames)
    {
        if (m_animationSequences.find(sequence) != m_animationSequences.end())
            {
                FE_LOG_WARNING("Animation sequence with name [", FE_GET_STR(sequence), "] [", sequence, "] already exists and is being overwritten");
            }

        m_animationSequences[sequence] = std::move(frames);
    }

const std::vector<fe::animationFrame> &fe::animationTexture::getAnimationSequence(fe::str str) const
    {
        if (m_animationSequences.find(str) == m_animationSequences.end())
            {
                FE_LOG_ERROR("Cannot get animation sequence with name [", FE_GET_STR(str), "] [", str, "]");
                fe::engine::crash("Animation failure");
            }
        return m_animationSequences.find(str)->second;
    }
