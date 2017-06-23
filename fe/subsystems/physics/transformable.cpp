#include "transformable.hpp"

fe::transformable::transformable() : 
    m_positionX(0.f),
    m_positionY(0.f),
    m_scaleX(0.f),
    m_scaleY(0.f),
    m_originX(0.f),
    m_originY(0.f),
    m_rotation(0.f), 
    m_update(true)
    {}

void fe::transformable::setPosition(float x, float y)
    {
        m_positionX = x;
        m_positionY = y;
        m_update = true;
    }

void fe::transformable::setRotation(float radians)
    {
        m_rotation = radians;
        m_update = true;
    }

void fe::transformable::setOrigin(float x, float y)
    {
        m_originX = x;
        m_originY = y;
        m_update |= true;
    }

void fe::transformable::setScale(float x, float y)
    {
        m_scaleX = x;
        m_scaleY = y;
        m_update |= true;
    }

void fe::transformable::setScale(float scale)
    {
        setScale(scale, scale);
    }

fe::Vector2d fe::transformable::getPosition() const
    {
        return { m_positionX, m_positionY };
    }

float fe::transformable::getRotation() const
    {
        return m_rotation;
    }

fe::Vector2d fe::transformable::getSize(float x, float y) const
    {
        return fe::Vector2d(m_scaleX * x, m_scaleY * y);
    }

fe::Vector2d fe::transformable::getScale() const
    {
        return { m_scaleX, m_scaleY };
    }

void fe::transformable::move(float x, float y)
    {
        setPosition(m_positionX + x, m_positionY + y);
        m_update = true;
    }

void fe::transformable::rotate(float radians)
    {
        m_rotation += radians;
        m_update |= (radians != 0.f);
    }

void fe::transformable::scale(float x, float y)
    {
        m_scaleX = x;
        m_scaleY = y;
        m_update = true;
    }

void fe::transformable::scale(float scale)
    {
        this->scale(scale, scale);
    }

const fe::matrix3d &fe::transformable::getMatrix()
    {
        if (m_update)
            {
                float cos = std::cos(m_rotation);
                float sin = std::sin(m_rotation);

                float scaleCosX = m_scaleX * cos;
                float scaleSinX = m_scaleX * sin;

                float scaleCosY = m_scaleY * cos;
                float scaleSinY = m_scaleY * sin;

                float tx = -m_originX * scaleCosX - m_originY * scaleSinY + m_positionX;
                float ty =  m_originX * scaleSinX - m_originY * scaleCosY + m_positionY;

                m_matrix = fe::matrix3d(scaleCosX, scaleSinY, 0.f,
                                       -scaleSinX, scaleCosY, 0.f,
                                        tx,        ty,        1.f);

                m_update = false;
            }

        return m_matrix;
    }
