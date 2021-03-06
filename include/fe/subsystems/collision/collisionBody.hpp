// collisionBody.hpp
// Multiple collision bodies that exist.
#pragma once
#include "collisionData.hpp"
#include "bounds/AABB.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/typeDefines.hpp"
#include "fe/subsystems/serializer/serializable.hpp"
#include "fe/subsystems/serializer/serializerID.hpp"
#include <functional>

namespace fe
    {
        struct collider : public fe::serializable
            {
                std::function<void(fe::collisionData&)> m_collisionCallback = [](fe::collisionData&) {};
                fe::str m_eventOnCollision = 0;
                fe::str m_collisionGroup;

                fe::AABB m_aabb;
                void *m_userData; // any additional data that needs to be stored
                void *m_metaData; // Memory that the user can use to store meta-data pertaining to the collider
                void *m_owner; // owner of the collider

                bool m_enabled = true;
                bool m_static;
                bool m_moved = true; // whether or not this collider moved last frame
                bool m_solid = true;

                void serialize(fe::serializerID &serializer) const
                    {
                        serializer.write("collisionGroup", m_collisionGroup);
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
                        m_collisionGroup = serializer.read<unsigned long>("collisionGroup");

                        serializer.readObject("bound", m_aabb);
                    }
            };
    }