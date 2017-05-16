// colliders.hpp
// all colliders in the game
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../math/Vector2.hpp"

#include <functional>

namespace fe
    {
        enum class colliderType
            {
                NONE,
                AABB,
            };

        struct collider
            {
                colliderType m_type;
                fe::Vector2d m_position;
                bool m_collisonEnabled;
                std::function<void(const collider&)> m_callback;

                collider() : m_type(colliderType::NONE), m_collisonEnabled(true)
                    {}
                virtual ~collider() = default;

                virtual void operator()(const collider&) const = 0;

                virtual bool collide(const collider &other) const = 0;
                virtual bool doesContain(const fe::Vector2d &point) const = 0;
                virtual bool doesRayIntersect(const Vector2d &origin, const Vector2d &direction) const = 0;
            };

        struct AABB : public collider
            {       
                FLAT_ENGINE_API AABB(const Vector2d size);
                FLAT_ENGINE_API AABB(const Vector2d size, std::function<void(const collider&)> callback);

                FLAT_ENGINE_API void operator()(const collider &collided) const { if (m_callback) m_callback(collided); }

                FLAT_ENGINE_API bool collide(const collider &other) const;
                FLAT_ENGINE_API bool doesContain(const fe::Vector2d &point) const;
                FLAT_ENGINE_API bool doesRayIntersect(const Vector2d &origin, const Vector2d &direction) const;

                Vector2d m_min;
                Vector2d m_max;
            };
    }