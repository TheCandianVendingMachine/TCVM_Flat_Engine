#include "transformable.hpp"

fe::transformable::transformable() : m_rotation(0.f), m_update(true), m_scale({ 1.f, 1.f })
    {}

void fe::transformable::setPosition(const fe::Vector2d &position)
    {
        m_position = position;
        m_update = true;
    }

void fe::transformable::setRotation(float radians)
    {
        m_rotation = radians;
        m_update = true;
    }

void fe::transformable::setOrigin(const fe::Vector2d &origin)
    {
        m_origin = origin;
        m_update |= true;
    }

void fe::transformable::setScale(const fe::Vector2d &scale)
    {
        m_scale = scale;
        m_update |= true;
    }

void fe::transformable::setScale(float scale)
    {
        setScale({ scale, scale });
    }

const fe::Vector2d &fe::transformable::getPosition() const
    {
        return m_position;
    }

float fe::transformable::getRotation() const
    {
        return m_rotation;
    }

fe::Vector2d fe::transformable::getSize(fe::Vector2d originalSize) const
    {
        return fe::Vector2d(m_scale.x * originalSize.x, m_scale.y * originalSize.y);
    }

const fe::Vector2d &fe::transformable::getScale() const
    {
        return m_scale;
    }

void fe::transformable::move(const fe::Vector2d &offset)
    {
        setPosition(getPosition() + offset);
        m_update |= (offset.magnitudeSqr() != 0.f);
    }

void fe::transformable::rotate(float radians)
    {
        m_rotation += radians;
        m_update |= (radians != 0.f);
    }

void fe::transformable::scale(const fe::Vector2d &scale)
    {
        m_scale = fe::Vector2d(scale.x * m_scale.x, scale.y * m_scale.y);
        m_update |= (scale.magnitudeSqr() != 0.f);
    }

void fe::transformable::scale(float scale)
    {
        return this->scale(fe::Vector2d(scale, scale));
    }

const fe::matrix3d &fe::transformable::getMatrix()
    {
        if (m_update)
            {
                float cos = std::cos(m_rotation);
                float sin = std::sin(m_rotation);

                float scaleCosX = m_scale.x * cos;
                float scaleSinX = m_scale.x * sin;

                float scaleCosY = m_scale.y * cos;
                float scaleSinY = m_scale.y * sin;

                float tx = -m_origin.x * scaleCosX - m_origin.y * scaleSinY + m_position.x;
                float ty =  m_origin.x * scaleSinX - m_origin.y * scaleCosY + m_position.y;

                m_matrix = fe::matrix3d(scaleCosX, scaleSinY, 0.f,
                                       -scaleSinX, scaleCosY, 0.f,
                                        tx,        ty,        1.f);

                m_update = false;
            }

        return m_matrix;
    }
