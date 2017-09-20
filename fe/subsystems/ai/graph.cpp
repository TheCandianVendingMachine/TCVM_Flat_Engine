#include "graph.hpp"
#include <algorithm>

int fe::graph::addNode(float posX, float posY, float cost)
    {
        m_nodes.emplace_back(new node);
        m_nodes.back()->m_gCost = cost;
        m_nodes.back()->m_posX = posX;
        m_nodes.back()->m_posY = posY;
        m_nodeHandles.push_back(m_nodes.size() - 1);
        return m_nodeHandles.size() - 1;
    }

void fe::graph::addEdge(unsigned int nodeA, unsigned int nodeB)
    {
        if (!(nodeA >= 0 && nodeB >= 0 && nodeA < m_nodes.size() && nodeB < m_nodes.size())) return;

        m_nodes[m_nodeHandles[nodeA]]->m_connectedNodes.push_back(nodeB);
        m_nodes[m_nodeHandles[nodeB]]->m_connectedNodes.push_back(nodeA);
    }

void fe::graph::removeEdge(unsigned int nodeID, unsigned int edge)
    {
        auto *node = m_nodes[m_nodeHandles[nodeID]].get();
        node->m_connectedNodes.erase(std::remove(node->m_connectedNodes.begin(), node->m_connectedNodes.end(), edge));
    }

fe::graph::node *fe::graph::getNode(unsigned int nodeID)
    {
        return m_nodes[m_nodeHandles[nodeID]].get();
    }

const std::vector<unsigned int> &fe::graph::getAllNodes()
    {
        return m_nodeHandles;
    }
