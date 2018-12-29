// collisionBounds.hpp
// A group of structs that represent different types of bounds
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/serializer/serializable.hpp"

namespace fe
    {
        struct collisionBounds : public fe::serializable
            {
                // Offsets are NOT added to the global position of the collision bound. Instead they are represented only during collision calculations
                float m_offsetX = 0.f;
                float m_offsetY = 0.f;
                float m_globalPositionX = 0.f;
                float m_globalPositionY = 0.f;

                virtual void serialize(fe::serializerID &serializer) const = 0;
                virtual void deserialize(fe::serializerID &serializer) = 0;
            };
    }