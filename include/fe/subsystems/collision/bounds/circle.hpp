// circle.hpp
// A simple circle bound
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/collision/bounds/collisionBounds.hpp"

namespace fe
    {
        struct circle : public fe::collisionBounds
            {
                float m_radius = 0.f;

                circle() {}
                circle(float radius) : m_radius(radius) {}

                FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
            };
    }
