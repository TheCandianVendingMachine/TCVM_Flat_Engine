#include "fe/subsystems/ai/graph.hpp"
#include <algorithm>

int fe::graph::addNode(float posX, float posY, float cost)
    {
        m_nodes.emplace_back(new node);
        m_nodes.back()->m_gCost = cost;
        m_nodes.back()->m_posX = posX;
        m_nodes.back()->m_posY = posY;
        m_nodeHandles.push_back(static_cast<int>(m_nodes.size() - 1));
        return static_cast<int>(m_nodeHandles.size() - 1);
    }

void fe::graph::addEdge(int nodeA, int nodeB)
    {
        if (!(nodeA >= 0 && nodeB >= 0 && nodeA < m_nodes.size() && nodeB < m_nodes.size())) return;

        m_nodes[m_nodeHandles[nodeA]]->m_connectedNodes.push_back(nodeB);
        m_nodes[m_nodeHandles[nodeB]]->m_connectedNodes.push_back(nodeA);
    }

void fe::graph::removeEdge(int nodeID, int edge)
    {
        if (edge < 0) return;

        auto *node = m_nodes[m_nodeHandles[nodeID]].get();
        node->m_connectedNodes.erase(std::remove(node->m_connectedNodes.begin(), node->m_connectedNodes.end(), edge));
    }

fe::graph::node *fe::graph::getNode(int nodeID)
    {
        return m_nodes[m_nodeHandles[nodeID]].get();
    }

const std::vector<int> &fe::graph::getAllNodes()
    {
        return m_nodeHandles;
    }
