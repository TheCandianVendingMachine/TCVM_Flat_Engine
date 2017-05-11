#include "animationTexture.hpp"
#include "animationActor.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

fe::animationTexture::animationTexture(const sf::Vector2u frameSize, const sf::Vector2u frameOffset, const sf::Vector2u animationSize, bool verticalStrip) :
    m_frameSize(frameSize),
    m_framePosition(frameOffset),
    m_vertical(verticalStrip),
    m_animationSize(animationSize)
    {
        m_maxFrames = m_vertical ? (m_animationSize.y - frameOffset.y) / m_frameSize.y : (m_animationSize.x - frameOffset.x) / m_frameSize.x;
    }

std::pair<sf::Vector2u, sf::Vector2u> fe::animationTexture::getTexture(animationActor *actor)
    {
        unsigned int maxFrame = m_maxFrames;
        if (maxFrame > actor->getEndFrame() && actor->getEndFrame() != 0)
            {
                maxFrame = actor->getEndFrame();
            }

        if (actor->getCurrentFrame() >= maxFrame || actor->getCurrentFrame() < actor->getStartFrame())
            {
                actor->setCurrentFrame(actor->getStartFrame());
            }

        sf::Vector2u frameOffset = m_framePosition;
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
