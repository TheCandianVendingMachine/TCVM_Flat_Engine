#include "fe/subsystems/collision/bounds/circle.hpp"
#include "fe/subsystems/serializer/serializerID.hpp"

void fe::circle::serialize(fe::serializerID &serializer) const
    {
        serializer.write("radius", m_radius);
        serializer.write("localPosX", m_offsetX);
        serializer.write("localPosY", m_offsetY);
        serializer.write("globalPosX", m_globalPositionX);
        serializer.write("globalPosY", m_globalPositionY);
    }

void fe::circle::deserialize(fe::serializerID &serializer)
    {
        m_radius = serializer.read<float>("radius");
        m_offsetX = serializer.read<float>("localPosX");
        m_offsetY = serializer.read<float>("localPosY");
        m_globalPositionX = serializer.read<float>("globalPosX");
        m_globalPositionY = serializer.read<float>("globalPosY");
    }
