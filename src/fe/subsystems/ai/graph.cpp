#include "fe/subsystems/ai/graph.hpp"
#include "fe/subsystems/serializer/serializerID.hpp"
#include <algorithm>

int fe::graph::addNode()
    {
        return addObject(new ai::node);
    }

void fe::graph::onAdd(ai::node **object, fe::Handle objectHandle)
    {
        (*object)->m_handle = objectHandle;
    }

int fe::graph::addNode(float posX, float posY, float cost)
    {
        int handle = addNode();
        m_objects.back()->m_gCost = cost;
        m_objects.back()->m_posX = posX;
        m_objects.back()->m_posY = posY;
        return handle;
    }

void fe::graph::removeNode(int nodeHandle)
    {
        ai::node *node = getObject(nodeHandle);

        for (auto &connection : node->m_connectedNodes)
            {
                removeEdge(connection, nodeHandle);
            }

        node->m_connectedNodes.clear();
        removeObject(nodeHandle);
    }

void fe::graph::addEdge(int nodeA, int nodeB)
    {
        if (!(nodeA >= 0 && nodeB >= 0 && nodeA < m_objects.size() && nodeB < m_objects.size())) return;

        getObject(nodeA)->m_connectedNodes.push_back(nodeB);
        getObject(nodeB)->m_connectedNodes.push_back(nodeA);
    }

void fe::graph::removeEdge(int nodeID, int edge)
    {
        if (edge < 0) return;

        auto *node = getObject(nodeID);
        node->m_connectedNodes.erase(std::remove(node->m_connectedNodes.begin(), node->m_connectedNodes.end(), edge), node->m_connectedNodes.end());
    }

fe::ai::node *fe::graph::getNode(int nodeID)
    {
        return getObject(nodeID);
    }

const std::vector<int> fe::graph::getAllNodes()
    {
        std::vector<int> nodes;

        for (auto &obj : getObjects())
            {
                nodes.push_back(getHandle(obj));
            }

        return nodes;
    }

void fe::graph::serialize(fe::serializerID &serializer) const
    {
        for (auto &node : getObjects())
            {
                serializer.writeObjectList("nodes", *node);
            }
    }

void fe::graph::deserialize(fe::serializerID &serializer)
    {
        while (serializer.listHasItems("nodes"))
            {
                ai::node tempData;
                serializer.readObjectList("nodes", tempData);

                ai::node *temp = getNode(addObject(new ai::node, tempData.m_handle));
                (*temp) = tempData;
            }
    }

void fe::ai::node::serialize(fe::serializerID &serializer) const
    {
        serializer.write("parent", m_parent);
        serializer.write("handle", m_handle);
        serializer.write("gCost", m_gCost);
        serializer.write("hCost", m_hCost);
        serializer.write("fCost", m_fCost);
        serializer.write("xPos", m_posX);
        serializer.write("yPos", m_posY);
        for (auto &connected : m_connectedNodes)
            {
                serializer.writeList("connected", connected);
            }
    }

void fe::ai::node::deserialize(fe::serializerID &serializer)
    {
        m_parent = serializer.read<int>("parent");
        m_handle = serializer.read<int>("handle");
        m_gCost = serializer.read<float>("gCost");
        m_hCost = serializer.read<float>("hCost");
        m_fCost = serializer.read<float>("fCost");
        m_posX = serializer.read<float>("xPos");
        m_posY = serializer.read<float>("yPos");
        while (serializer.listHasItems("connected"))
            {
                m_connectedNodes.push_back(serializer.readList<int>("connected"));
            }
    }

fe::ai::node &fe::ai::node::operator=(const node &rhs)
    {
        if (this != &rhs)
            {
                this->m_handle = rhs.m_handle;
                this->m_parent = rhs.m_parent;

                this->m_gCost = rhs.m_gCost;
                this->m_fCost = rhs.m_fCost;
                this->m_hCost = rhs.m_hCost;

                this->m_posX = rhs.m_posX;
                this->m_posY = rhs.m_posY;

                this->m_userData = rhs.m_userData;

                m_connectedNodes = rhs.m_connectedNodes;
            }

        return *this;
    }
