#include "fe/subsystems/physics/collision/collisionBounds.hpp"
#include "fe/subsystems/serializer/serializerID.hpp"
#include <algorithm>

bool fe::AABB::intersects(const collisionBounds &other) const
    {
        return intersects(static_cast<const fe::AABB&>(other));
    }

bool fe::AABB::contains(const collisionBounds &other) const
    {
        return contains(static_cast<const fe::AABB&>(other));
    }

bool fe::AABB::contains(float x, float y) const
    {
        return x >= m_globalPositionX && y >= m_globalPositionY && x < m_globalPositionX + m_sizeX && y < m_globalPositionY + m_sizeY;
    }

bool fe::AABB::intersects(const fe::AABB &other) const
    {
        if (m_globalPositionX + m_sizeX < other.m_globalPositionX || m_globalPositionX > other.m_globalPositionX + other.m_sizeX) return false;
        if (m_globalPositionY + m_sizeY < other.m_globalPositionY || m_globalPositionY > other.m_globalPositionY + other.m_sizeY) return false;
        return true;
    }

bool fe::AABB::contains(const fe::AABB &other) const
    {
        return  (other.m_globalPositionX >= m_globalPositionX && other.m_globalPositionX + other.m_sizeX <= m_globalPositionX + m_sizeX) &&
                (other.m_globalPositionY >= m_globalPositionY && other.m_globalPositionY + other.m_sizeY <= m_globalPositionY + m_sizeY);
    }

fe::AABB fe::AABB::merge(const fe::AABB &other)
    {
        fe::AABB aabb;
        aabb.m_globalPositionX = std::min(m_globalPositionX, other.m_globalPositionX);
        aabb.m_globalPositionY = std::min(m_globalPositionY, other.m_globalPositionY);
        aabb.m_sizeX = std::max(m_globalPositionX + m_sizeX, other.m_globalPositionX + other.m_sizeX) - std::min(m_globalPositionX, other.m_globalPositionX);
        aabb.m_sizeY = std::max(m_globalPositionY + m_sizeY, other.m_globalPositionY + other.m_sizeY) - std::min(m_globalPositionY, other.m_globalPositionY);
        return aabb;
    }

float fe::AABB::volume() const
    {
        return m_sizeX * m_sizeY;
    }

float fe::AABB::perimeter() const
    {
        return 2.f * (m_sizeX + m_sizeY);
    }

void fe::AABB::serialize(fe::serializerID &serializer) const
    {
        serializer.write("sizeX", m_sizeX);
        serializer.write("sizeY", m_sizeY);
        serializer.write("localPosX", m_offsetX);
        serializer.write("localPosY", m_offsetY);
        serializer.write("globalPosX", m_globalPositionX);
        serializer.write("globalPosY", m_globalPositionY);
    }

void fe::AABB::deserialize(fe::serializerID &serializer)
    {
        m_sizeX = serializer.read<float>("sizeX");
        m_sizeY = serializer.read<float>("sizeY");
        m_offsetX = serializer.read<float>("localPosX");
        m_offsetY = serializer.read<float>("localPosY");
        m_globalPositionX = serializer.read<float>("globalPosX");
        m_globalPositionY = serializer.read<float>("globalPosY");
    }
