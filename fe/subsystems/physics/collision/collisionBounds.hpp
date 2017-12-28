// collisionBounds.hpp
// A group of structs that represent different types of bounds
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../serializer/serializerID.hpp"

namespace fe
    {
        struct collisionBounds
            {
                float m_positionX = 0.f;
                float m_positionY = 0.f;
                float m_globalPositionX = 0.f;
                float m_globalPositionY = 0.f;

                virtual bool intersects(const collisionBounds &other) const = 0;
                virtual bool contains(const collisionBounds &other) const = 0;
                virtual bool contains(float x, float y) const = 0;
            };

        struct AABB : collisionBounds
            {
                float m_sizeX = 0.f;
                float m_sizeY = 0.f;

                FLAT_ENGINE_API bool intersects(const collisionBounds &other) const;
                FLAT_ENGINE_API bool contains(const collisionBounds &other) const;
                FLAT_ENGINE_API bool contains(float x, float y) const;

                FLAT_ENGINE_API bool intersects(const fe::AABB &other) const;
                FLAT_ENGINE_API bool contains(const fe::AABB &other) const;
                FLAT_ENGINE_API fe::AABB merge(const fe::AABB &other);
                FLAT_ENGINE_API float volume() const;
                FLAT_ENGINE_API float perimeter() const;

                SERIALIZE_ID("aabb", "sizeX", m_sizeX, "sizeY", m_sizeY, "posX", m_positionX, "posY", m_positionY, "globalPosX", m_globalPositionX, "globalPosY", m_globalPositionY);
            };
    }