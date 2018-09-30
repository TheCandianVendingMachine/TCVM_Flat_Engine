#include "fe/subsystems/graphic/animation/animationActor.hpp"
#include "fe/subsystems/graphic/renderObject/renderObject.hpp"

fe::animationActor::animationActor(fe::renderObject *const actor) :
    m_actorVerticies(actor),
    m_animationFrameSpeed(0),
    m_currentFrame(0),
    m_endFrame(0),
    m_startFrame(0),
    m_play(false),
    m_lastCheckedTime(0)
    {
    }

void fe::animationActor::play(bool value)
    {
        m_play = value;
        if (!m_play)
            {
                m_pauseTime = fe::clock::getTimeSinceEpoch();
            }
        else if (m_pauseTime > 0)
            {
                m_lastCheckedTime += fe::clock::getTimeSinceEpoch() - m_pauseTime;
            }
    }

bool fe::animationActor::isPlaying() const
    {
        return m_play;
    }

bool fe::animationActor::needsUpdate(fe::time elapsedTime)
    {
        if (m_play) 
            {
                m_needsUpdate = (int)(m_animationFrameSpeed - (elapsedTime - m_lastCheckedTime).asMilliseconds()) <= 0;
                if (m_needsUpdate)
                    {
                        m_lastCheckedTime = elapsedTime;
                    }
            }
        return m_needsUpdate;
    }

void fe::animationActor::setFrameSpeed(unsigned int animationSpeed)
    {
        m_animationFrameSpeed = animationSpeed;
    }

unsigned int fe::animationActor::getFrameSpeed() const
    {
        return m_animationFrameSpeed;
    }

void fe::animationActor::setEndFrame(unsigned int maxFrames)
    {
        m_endFrame = maxFrames;
    }

unsigned int fe::animationActor::getEndFrame()
    {
        return m_endFrame;
    }

void fe::animationActor::setStartFrame(unsigned int frame)
    {
        m_startFrame = frame;
    }

unsigned int fe::animationActor::getStartFrame()
    {
        return m_startFrame;
    }

void fe::animationActor::setCurrentFrame(unsigned int frame)
    {
        m_currentFrame = frame;
        if (m_currentFrame > m_endFrame && m_endFrame != 0)
            {
                setCurrentFrame(m_currentFrame % m_endFrame);
            }
        
        if (m_currentFrame < m_startFrame)
            {
                setCurrentFrame(m_startFrame);
            }
        m_needsUpdate = true;
    }

unsigned int fe::animationActor::getCurrentFrame() const
    {
        return m_currentFrame;
    }

void fe::animationActor::iterateFrame(int amount)
    {
        setCurrentFrame(m_currentFrame + amount);
    }

void fe::animationActor::updateVerticies(fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> size)
    {
        m_needsUpdate = false;
        m_actorVerticies->m_texCoords[0] = offset.x;
        m_actorVerticies->m_texCoords[1] = offset.y;

        m_actorVerticies->m_texCoords[2] = size.x;
        m_actorVerticies->m_texCoords[3] = size.y;
    }