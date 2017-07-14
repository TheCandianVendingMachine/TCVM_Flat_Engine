#include "aabbTree.hpp"

fe::aabbTree::node::node()
    {
    }

bool fe::aabbTree::node::isLeaf()
    {
        return false;
    }

void fe::aabbTree::node::setBranch(node * n0, node * n1)
    {
    }

void fe::aabbTree::node::setLeaf(fe::AABB * data)
    {
    }

void fe::aabbTree::node::updateAABB(float margin)
    {
    }

fe::aabbTree::aabbTree() : m_root(nullptr), m_margin(0.2f)
    {
    }
