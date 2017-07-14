#include "aabbTree.hpp"
#include <algorithm>

fe::aabbTree::node::node() : parent(nullptr), data(nullptr)
    {
        children[0] = nullptr;
        children[1] = nullptr;
    }

bool fe::aabbTree::node::isLeaf() const
    {
        return children[0] == nullptr;
    }

void fe::aabbTree::node::setBranch(node *n0, node *n1)
    {
        n0->parent = this;
        n1->parent = this;

        children[0] = n0;
        children[1] = n1;
    }

void fe::aabbTree::node::setLeaf(fe::AABB *data)
    {
        this->data = data;

        children[0] = nullptr;
        children[1] = nullptr;
    }

void fe::aabbTree::node::updateAABB(float margin)
    {
        if (isLeaf())
            {
                aabb.m_positionX = data->m_positionX - margin;
                aabb.m_positionY = data->m_positionY - margin;

                aabb.m_sizeX = data->m_sizeX + margin;
                aabb.m_sizeY = data->m_sizeY + margin;
            }
        else
            {
                aabb.m_positionX = std::min(children[0]->aabb.m_positionX, children[1]->aabb.m_positionX);
                aabb.m_positionY = std::min(children[0]->aabb.m_positionY, children[1]->aabb.m_positionY);

                float lowestMaxX = children[0]->aabb.m_positionX < children[1]->aabb.m_positionX ? children[0]->aabb.m_positionX : children[1]->aabb.m_positionX;
                float highestMinX = children[0]->aabb.m_positionX > children[1]->aabb.m_positionX ? children[0]->aabb.m_positionX : children[1]->aabb.m_positionX;

                float lowestMaxY = children[0]->aabb.m_positionY < children[1]->aabb.m_positionY ? children[0]->aabb.m_positionY : children[1]->aabb.m_positionY;
                float highestMinY = children[0]->aabb.m_positionY > children[1]->aabb.m_positionY ? children[0]->aabb.m_positionY : children[1]->aabb.m_positionY;

                aabb.m_sizeX = children[0]->aabb.m_sizeX + children[1]->aabb.m_sizeX + (highestMinX - lowestMaxX);
                aabb.m_sizeY = children[0]->aabb.m_sizeY + children[1]->aabb.m_sizeY + (highestMinY - lowestMaxY);
            }
    }

fe::aabbTree::node *fe::aabbTree::node::getSibling() const
    {
        return this == parent->children[0] ? parent->children[1] : parent->children[0];
    }

fe::aabbTree::aabbTree() : m_root(nullptr), m_margin(0.2f)
    {
    }
