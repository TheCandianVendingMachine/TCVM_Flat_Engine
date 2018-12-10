// AABB.hpp
// An axis-aligned bounding box
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/serializer/serializable.hpp"
#include "collisionBounds.hpp"

namespace fe
    {
        struct AABB : public collisionBounds
            {
                float m_sizeX = 0.f;
                float m_sizeY = 0.f;

                AABB() {}
                AABB(float sizeX, float sizeY) : m_sizeX(sizeX), m_sizeY(sizeY) {}

                FLAT_ENGINE_API fe::AABB merge(const fe::AABB &other);
                FLAT_ENGINE_API float volume() const;
                FLAT_ENGINE_API float perimeter() const;

                FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
            };
    }