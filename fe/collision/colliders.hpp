// colliders.hpp
// all colliders in the game
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"

#include "../math/Vector2.hpp"
#include "../misc/function.hpp"

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

                collider() : m_type(colliderType::NONE), m_collisonEnabled(true)
                    {}

                virtual void operator()(const collider&) const = 0;

                virtual bool collide(const collider &other) const = 0;
                virtual bool doesContain(const fe::Vector2d &point) const = 0;
                virtual bool doesRayIntersect(const Vector2d &origin, const Vector2d &direction) const = 0;
            };

        template<typename Obj>
        struct AABB : public collider
            {
                fe::function<void, Obj, const collider&> m_callback;
            
                AABB(const Vector2d &position) : collider(position) {}

                // If the object has an instance, provide one
                AABB(const Vector2d &size, Obj *instance = nullptr) : m_max(size) 
                    {
                        m_type = colliderType::AABB;
                        if (instance)
                            {
                                m_callback = fe::function<void, Obj, const fe::collider&>(fe::fPtr<true, Obj, void, const fe::collider&>(instance, &Obj::collision));
                            }
                    }

                void operator()(const collider &collided) const { m_callback(collided); }

                FLAT_ENGINE_API bool collide(const collider &other) const;
                FLAT_ENGINE_API bool doesContain(const fe::Vector2d &point) const;
                FLAT_ENGINE_API bool doesRayIntersect(const Vector2d &origin, const Vector2d &direction) const;

                Vector2d m_min;
                Vector2d m_max;
            };
    }

#include "colliders.inl"