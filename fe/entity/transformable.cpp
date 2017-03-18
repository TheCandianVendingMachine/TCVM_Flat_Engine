#include "transformable.hpp"

fe::transformable::transformable() : m_rotation(0.f)
    {}

void fe::transformable::setPosition(const fe::Vector2d &position)
    {
        m_position = position;
    }

void fe::transformable::setRotation(const float &radians)
    {
        m_rotation = radians;
    }

void fe::transformable::setOrigin(const fe::Vector2d &origin)
    {
        m_matrix.origin = origin;
    }

fe::Vector2d fe::transformable::getPosition()
    {
        return m_position;
    }

float fe::transformable::getRotation()
    {
        return m_rotation;
    }

void fe::transformable::move(const fe::Vector2d &offset)
    {
        m_position += offset;
    }

void fe::transformable::rotate(const float &radians)
    {
        m_rotation += radians;
    }

const fe::matrix3d &fe::transformable::getMatrix()
    {
        float cos = std::cos(m_rotation);
        float sin = std::sin(m_rotation);

        float tx = m_position.x ;
        float ty = m_position.y;

        m_matrix = fe::matrix3d(cos, sin, 0.f,
                               -sin, cos, 0.f,
                                tx, ty,   1.f);

        return m_matrix;
    }
