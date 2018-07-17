#include "fe/subsystems/graphic/animation/animationTexture.hpp"
#include "fe/subsystems/graphic/animation/animationActor.hpp"
#include "fe/debug/logger.hpp"

#include <SFML/Graphics/Texture.hpp>

fe::animationTexture::animationTexture(const fe::Vector2<unsigned int> frameSize, const fe::Vector2<unsigned int> frameOffset, const fe::Vector2<unsigned int> animationSize, bool verticalStrip) :
    m_frameSize(frameSize),
    m_framePosition(frameOffset),
    m_vertical(verticalStrip),
    m_animationSize(animationSize)
    {
        if (m_frameSize.x != 0 && m_frameSize.y != 0)
            {
                m_maxFrames = m_vertical ? (m_animationSize.y - frameOffset.y) / m_frameSize.y : (m_animationSize.x - frameOffset.x) / m_frameSize.x;
            }
        else
            {
                FE_LOG_ERROR("Cannot load texture into animation");
            }
    }

std::pair<fe::Vector2<unsigned int>, fe::Vector2<unsigned int>> fe::animationTexture::getTexture(animationActor *actor)
    {
        unsigned int maxFrame = m_maxFrames;
        if (maxFrame > actor->getEndFrame() && actor->getEndFrame() != 0)
            {
                maxFrame = actor->getEndFrame();
            }

        if (actor->getCurrentFrame() > maxFrame || actor->getCurrentFrame() < actor->getStartFrame())
            {
                actor->setCurrentFrame(actor->getStartFrame());
            }

        fe::Vector2<unsigned int> frameOffset = m_framePosition;
        if (m_vertical)
            {
                frameOffset.y += actor->getCurrentFrame() * m_frameSize.y;
            }
        else
            {
                frameOffset.x += actor->getCurrentFrame() * m_frameSize.x;
            }

        return std::make_pair(frameOffset, m_frameSize);
    }
