#include "animationActor.hpp"
#include "../../../time/clock.hpp"
#include <SFML/Graphics/VertexArray.hpp>

fe::animationActor::animationActor(sf::VertexArray *verticies) :
    m_verticies(verticies),
    m_animationFrameSpeed(0.f),
    m_currentFrame(0),
    m_endFrame(0),
    m_startFrame(0),
    m_play(false)
    {
    }

void fe::animationActor::play(bool value)
    {
        m_play = value;
        if (!m_play)
            {
                m_pauseTime = fe::clock::getTimeSinceEpoch();
            }
        else
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
        if (!m_play) return false;

        bool update = (int)(m_animationFrameSpeed - (elapsedTime - m_lastCheckedTime).asMilliseconds()) <= 0;
        if (update) 
            {
                m_lastCheckedTime = elapsedTime;
            }
        return update;
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
    }

unsigned int fe::animationActor::getCurrentFrame() const
    {
        return m_currentFrame;
    }

void fe::animationActor::iterateFrame(int amount)
    {
        setCurrentFrame(m_currentFrame + amount);
        if (m_currentFrame > m_endFrame && m_endFrame != 0)
            {
                setCurrentFrame(m_currentFrame % m_endFrame);
            }
        
        if (m_currentFrame < m_startFrame)
            {
                setCurrentFrame(m_startFrame);
            }
    }

void fe::animationActor::updateVerticies(fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> size)
    {
        (*m_verticies)[0].position = sf::Vector2f(0, 0);
        (*m_verticies)[1].position = sf::Vector2f(size.x, 0);
        (*m_verticies)[2].position = sf::Vector2f(size.x, size.y);
        (*m_verticies)[3].position = sf::Vector2f(0, size.y);

        (*m_verticies)[0].texCoords = sf::Vector2f(offset.x, offset.y);
        (*m_verticies)[1].texCoords = sf::Vector2f(offset.x + size.x, offset.y);
        (*m_verticies)[2].texCoords = sf::Vector2f(offset.x + size.x, offset.y + size.y);
        (*m_verticies)[3].texCoords = sf::Vector2f(offset.x, offset.y + size.y);
    }