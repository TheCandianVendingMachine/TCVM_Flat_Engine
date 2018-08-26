// collisionBounds.hpp
// A group of structs that represent different types of bounds
#pragma once
#include "../../../flatEngineExport.hpp"
#include "../../serializer/serializable.hpp"

namespace fe
    {
        struct collisionBounds : public fe::serializable
            {
                float m_offsetX = 0.f; // position of the collider on the entity
                float m_offsetY = 0.f; // position of the collider on the entity
                float m_globalPositionX = 0.f;
                float m_globalPositionY = 0.f;

                virtual bool intersects(const collisionBounds &other) const = 0;
                virtual bool contains(const collisionBounds &other) const = 0;
                virtual bool contains(float x, float y) const = 0;

                virtual void serialize(fe::serializerID &serializer) const = 0;
                virtual void deserialize(fe::serializerID &serializer) = 0;
            };

        struct AABB : public collisionBounds
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

                FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
            };
    }