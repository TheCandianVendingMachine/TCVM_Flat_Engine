#include "fe/subsystems/physics/transformable.hpp"

fe::transformable::transformable() : 
    m_positionX(0.f),
    m_positionY(0.f),
    m_scaleX(1.f),
    m_scaleY(1.f),
    m_originX(0.f),
    m_originY(0.f),
    m_rotation(0.f), 
    m_update(true),
    m_updateChildren(true)
    {}

void fe::transformable::clear()
    {
        m_matrix = fe::matrix3d();
    }

void fe::transformable::setPosition(float x, float y)
    {
        m_update = abs(m_positionX - x) > 0.00001f || abs(m_positionY - y) > 0.00001f;
        m_positionX = x;
        m_positionY = y;
        m_update = true;
        m_updateChildren = true;
    }

void fe::transformable::setPosition(fe::Vector2d pos)
	{
		setPosition(pos.x, pos.y);
	}

void fe::transformable::setRotation(float radians)
    {
        m_rotation = radians;
        m_update = true;
        m_updateChildren = true;
    }

void fe::transformable::setOrigin(float x, float y)
    {
        m_originX = x;
        m_originY = y;
        m_update = true;
        m_updateChildren = true;
    }

void fe::transformable::setOrigin(fe::Vector2d origin)
	{
		setOrigin(origin.x, origin.y);
	}

void fe::transformable::setScale(float x, float y)
    {
        m_scaleX = x;
        m_scaleY = y;
        m_update = true;
        m_updateChildren = true;
    }

void fe::transformable::setScale(fe::Vector2d scale)
	{
		setScale(scale.x, scale.y);
	}

void fe::transformable::setScale(float scale)
    {
        setScale(scale, scale);
    }

fe::lightVector2d fe::transformable::getPosition() const
    {
        return { m_positionX, m_positionY };
    }

float fe::transformable::getRotation() const
    {
        return m_rotation;
    }

fe::lightVector2d fe::transformable::getSize(float x, float y) const
    {
        return fe::lightVector2d(m_scaleX * x, m_scaleY * y);
    }

fe::lightVector2d fe::transformable::getOrigin() const
    {
        return fe::lightVector2d(m_originX, m_originY);
    }

fe::lightVector2d fe::transformable::getScale() const
    {
        return { m_scaleX, m_scaleY };
    }

void fe::transformable::move(float x, float y)
    {
        setPosition(m_positionX + x, m_positionY + y);
    }

void fe::transformable::move(fe::Vector2d offset)
	{
		move(offset.x, offset.y);
	}

void fe::transformable::rotate(float radians)
    {
        m_rotation += radians;
        m_update = true;
        m_updateChildren = true;
    }

void fe::transformable::scale(float x, float y)
    {
        m_scaleX = x;
        m_scaleY = y;
        m_update = true;
        m_updateChildren = true;
    }

void fe::transformable::scale(fe::Vector2d scaley)
	{
		scale(scaley.x, scaley.y);
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

void fe::transformable::combine(fe::transformable &other)
    {
        getMatrix();
        m_matrix.combine(other.getMatrix());
        m_positionX = m_matrix[6];
        m_positionY = m_matrix[7];
        m_rotation += other.m_rotation;
    }

bool fe::transformable::updateChildren() const
    {
        return m_updateChildren;
    }

void fe::transformable::setUpdateChildren(bool value)
    {
        m_updateChildren = value;
    }

fe::transformable &fe::transformable::operator=(const fe::transformable &rhs)
    {
        if (&rhs != this)
            {
                setPosition(rhs.getPosition().x, rhs.getPosition().y);
                setScale(rhs.getScale().x, rhs.getScale().y);
                setRotation(rhs.getRotation());
                setOrigin(rhs.getOrigin().x, rhs.getOrigin().y);

                m_matrix = rhs.m_matrix;
            }

        return *this;
    }
