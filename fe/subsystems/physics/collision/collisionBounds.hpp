// collisionBounds.hpp
// A group of structs that represent different types of bounds
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

namespace fe
    {
        struct collisionBounds
            {
                float m_positionX = 0.f;
                float m_positionY = 0.f;

                virtual bool intersects(const collisionBounds &other) const = 0;
                virtual bool contains(const collisionBounds &other) const = 0;
            };

        struct AABB : collisionBounds
            {
                float m_sizeX = 0.f;
                float m_sizeY = 0.f;

                FLAT_ENGINE_API bool intersects(const collisionBounds &other) const;
                FLAT_ENGINE_API bool contains(const collisionBounds &other) const;

                FLAT_ENGINE_API bool intersects(const fe::AABB &other) const;
                FLAT_ENGINE_API bool contains(const fe::AABB &other) const;
                FLAT_ENGINE_API fe::AABB merge(const fe::AABB &other);
                FLAT_ENGINE_API float volume() const;
                FLAT_ENGINE_API float perimeter() const;
            };
    }