// collisionBounds.hpp
// A group of structs that represent different types of bounds
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/serializer/serializable.hpp"

namespace fe
    {
        struct collisionBounds : public fe::serializable
            {
                float m_offsetX = 0.f; // position of the collider on the entity
                float m_offsetY = 0.f; // position of the collider on the entity
                float m_globalPositionX = 0.f;
                float m_globalPositionY = 0.f;

                virtual void serialize(fe::serializerID &serializer) const = 0;
                virtual void deserialize(fe::serializerID &serializer) = 0;
            };
    }