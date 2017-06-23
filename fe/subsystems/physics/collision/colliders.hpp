// colliders.hpp
// all colliders in the game
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../math/Vector2.hpp"
#include "collisionData.hpp"

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
                std::function<void(fe::collisionData)> m_callback;

                collider() : m_type(colliderType::NONE), m_collisonEnabled(true)
                    {}
                virtual ~collider() = default;

                virtual void operator()(fe::collisionData) const = 0;

                virtual bool collide(const collider &other) const = 0;
                virtual bool doesContain(const fe::Vector2d &point) const = 0;
                virtual bool doesRayIntersect(const Vector2d &origin, const Vector2d &direction) const = 0;

                virtual fe::collisionData getCollisionData(const collider &other) = 0;
            };

        struct AABB : public collider
            {       
                FLAT_ENGINE_API AABB(const Vector2d size);
                FLAT_ENGINE_API AABB(const Vector2d size, std::function<void(fe::collisionData)> callback);

                FLAT_ENGINE_API void operator()(fe::collisionData collided) const { if (m_callback) m_callback(collided); }

                FLAT_ENGINE_API bool collide(const collider &other) const;
                FLAT_ENGINE_API bool doesContain(const fe::Vector2d &point) const;
                FLAT_ENGINE_API bool doesRayIntersect(const Vector2d &origin, const Vector2d &direction) const;

                FLAT_ENGINE_API fe::collisionData getCollisionData(const collider &other);

                Vector2d m_min; // min position. Should always be 0,0 for this type
                Vector2d m_max; // max position. Size can be found with m_max - m_min
            };
    }