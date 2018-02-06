// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "collisionData.hpp"
#include "collisionBounds.hpp"
#include "../../../math/Vector2.hpp"
#include "../../../typeDefines.hpp"
#include "../../serializer/serializable.hpp"
#include "../../serializer/serializerID.hpp"
#include <functional>

namespace fe
    {
        struct collider : public fe::serializable
            {
                std::function<void(fe::collisionData&)> m_collisionCallback = [](fe::collisionData&) {};
                fe::str m_eventOnCollision;

                AABB m_aabb;
                void *m_userData; // any additional data that needs to be stored

                bool m_enabled = true;
                bool m_static;

                void serialize(fe::serializerID &serializer) const
                    {
                        serializer.write("eventOnCollision", m_eventOnCollision);
                        serializer.write("static", m_static);
                        serializer.write("enabled", m_enabled);
                        serializer.writeObject("bound", m_aabb);
                    }

                void deserialize(fe::serializerID &serializer)
                    {
                        m_eventOnCollision = serializer.read<fe::str>("eventOnCollision");
                        m_static = serializer.read<bool>("static");
                        m_enabled = serializer.read<bool>("enabled");

                        serializer.readObject("bound", m_aabb);
                    }
            };
    }