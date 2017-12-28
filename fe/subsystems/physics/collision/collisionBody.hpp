// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "collisionData.hpp"
#include "collisionBounds.hpp"
#include "../../../math/Vector2.hpp"
#include "../../../typeDefines.hpp"
#include "../../serializer/serializerID.hpp"
#include <functional>

namespace fe
    {
        struct collider 
            {
                std::function<void(fe::collisionData&)> m_collisionCallback = [](fe::collisionData&) {};
                fe::str m_eventOnCollision;

                AABB m_aabb;
                void *m_userData; // any additional data that needs to be stored

                bool m_enabled = true;
                bool m_static;

                SERIALIZE_ID("collider", "eventOnCollision", m_eventOnCollision, "static", m_static, "enabled", m_enabled, "bound", m_aabb);
            };
    }