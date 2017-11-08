#include "tree.hpp"
#include <algorithm>

fe::tree::tree()
    {
    }

int fe::tree::addNode()
    {
        return addObject(new priv::node);
    }

void fe::tree::removeNode(int node)
    {
        if (node < 0) return;
        removeChild(getObject(node)->m_parent, node);
        for (auto &childChild : getObject(node)->m_children)
            {
                getObject(childChild)->m_parent = getObject(node)->m_parent;
            }
        removeObject(node);
    }

void fe::tree::addChild(int node, int child)
    {
        if (node < 0 || child < 0) return;
        getObject(node)->m_children.push_back(child);
        getObject(child)->m_parent = node;
    }

void fe::tree::removeChild(int node, int child)
    {
        if (node < 0 || child < 0) return;
        priv::node *nodePtr = getObject(node);
        nodePtr->m_children.erase(std::remove(nodePtr->m_children.begin(), nodePtr->m_children.end(), child), nodePtr->m_children.end());
        getObject(child)->m_parent = -1;
    }

fe::priv::node *fe::tree::getNode(int node)
    {
        if (node < 0) return nullptr;
        return getObject(node);
    }

bool fe::tree::nodeExists(int node, int nodeCheck)
    {
        std::vector<int> &children = getNode(node)->m_children;
        return std::find(children.begin(), children.end(), nodeCheck) != children.end();
    }

void fe::tree::sort(int node)
    {
        std::vector<int> *children = &getNode(node)->m_children;
        std::sort(children->begin(), children->end(), std::less<int>());
    }

void fe::tree::sort(int node, std::function<bool(int, int)> check)
    {
        std::vector<int> *children = &getNode(node)->m_children;
        std::sort(children->begin(), children->end(), check);
    }
