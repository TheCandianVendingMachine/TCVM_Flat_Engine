// node.hpp
// AI node for pathfinding
#pragma once
#include "../../flatEngineExport.hpp"
#include "../serializer/serializable.hpp"
#include "../../typeDefines.hpp"
#include <vector>

namespace fe
    {
        namespace ai
            {
                struct node : public fe::serializable
                    {
                        // Nodes this node is connected to
                        std::vector<int> m_connectedNodes;
                        fe::Handle m_handle;
                        int m_parent; // used for pathfinding

                        float m_gCost;
                        float m_hCost;
                        float m_fCost;

                        float m_posX;
                        float m_posY;

                        void *m_userData;

                        node() : m_parent(-1), m_gCost(-1.f), m_hCost(-1.f), m_fCost(-1.f), m_posX(0.f), m_posY(0.f) {}

                        FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                        FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

                        FLAT_ENGINE_API node &operator=(const node &rhs);
                    };
            }
    }
