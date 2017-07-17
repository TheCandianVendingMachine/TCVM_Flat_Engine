// collisionBounds.hpp
// A group of structs that represent different types of bounds
#pragma once
#define FE_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

namespace fe
    {
        struct collisionBounds
            {
                float m_positionX;
                float m_positionY;

                virtual bool intersects(const collisionBounds *other) const = 0;
                virtual bool contains(const collisionBounds *other) const = 0;
            };

        struct AABB : collisionBounds
            {
                float m_sizeX;
                float m_sizeY;

                FLAT_ENGINE_API bool intersects(const collisionBounds *other) const;
                FLAT_ENGINE_API bool contains(const collisionBounds *other) const;

                FLAT_ENGINE_API bool intersects(const fe::AABB *other) const;
                FLAT_ENGINE_API bool contains(const fe::AABB *other) const;
                FLAT_ENGINE_API fe::AABB merge(const fe::AABB *other);
                FLAT_ENGINE_API float volume() const;
            };
    }