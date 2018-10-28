#include "fe/subsystems/graphic/animation/animationActor.hpp"
#include "fe/subsystems/graphic/animation/animationTexture.hpp"
#include "fe/subsystems/graphic/renderObject/renderObject.hpp"

fe::animationActor::animationActor(fe::renderObject *const actor) :
    m_actorVerticies(actor),
    m_play(false),
    m_currentTime(0),
    m_currentFrame(0, 0),
    m_currentAnimationSequence(0),
    m_currentSequenceFrame(0),
    m_playingSequence(false),
    m_doneSequence(true)
    {
    }

void fe::animationActor::play(bool value)
    {
        m_play = value;
    }

void fe::animationActor::stop()
    {
        m_playingSequence = false;
    }

bool fe::animationActor::isPlaying() const
    {
        return m_play;
    }

bool fe::animationActor::isPlayingSequence() const
    {
        return m_playingSequence && !m_doneSequence;
    }

void fe::animationActor::update(fe::animationTexture &texture, fe::time elapsedTime)
    {
        m_currentTime = elapsedTime;
        if (m_play && isPlayingSequence())
            {
                if ((texture.getAnimationSequence(m_currentAnimationSequence)[m_currentSequenceFrame].m_time + m_startTime - m_currentTime) <= 0)
                    {
                        m_doneSequence = (m_currentSequenceFrame + 1) >= texture.getAnimationSequence(m_currentAnimationSequence).size();
                        m_playingSequence = !m_doneSequence;
                        m_updateFrame = m_playingSequence;
                        m_currentSequenceFrame++;
                    }

                if (m_updateFrame) 
                    {
                        m_currentFrame = texture.getAnimationSequence(m_currentAnimationSequence)[m_currentSequenceFrame].m_frame;
                        auto textureOffset = texture.getTexture(m_currentFrame.x, m_currentFrame.y);
                        updateVerticies(textureOffset.first, textureOffset.second);
                        m_updateFrame = false;
                    }
            }
    }

void fe::animationActor::setFrameSpeed(unsigned int animationSpeed)
    {
    }

unsigned int fe::animationActor::getFrameSpeed() const
    {
        return 0;
    }

void fe::animationActor::setCurrentFrame(unsigned int x, unsigned int y)
    {
        m_currentFrame.x = x;
        m_currentFrame.y = y;
    }

fe::Vector2<unsigned int> fe::animationActor::getCurrentFrame() const
    {
        return m_currentFrame;
    }

void fe::animationActor::updateVerticies(fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> size)
    {
        m_actorVerticies->m_texCoords[0] = offset.x;
        m_actorVerticies->m_texCoords[1] = offset.y;

        m_actorVerticies->m_texCoords[2] = size.x;
        m_actorVerticies->m_texCoords[3] = size.y;
    }

void fe::animationActor::playSequence(fe::str sequence)
    {
        m_currentAnimationSequence = sequence;
        m_doneSequence = false;
        m_playingSequence = true;
        m_currentSequenceFrame = 0;
        m_startTime = m_currentTime;
        m_updateFrame = true;
    }
