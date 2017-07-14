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

void fe::aabbTree::updateNodeHelper(node *base, std::vector<node*> &invalidNodes)
    {
        if (base->isLeaf())
            {
                if (!base->aabb.contains(base->data))
                    {
                        invalidNodes.push_back(base);
                    }
            }
        else
            {
                updateNodeHelper(base->children[0], invalidNodes);
                updateNodeHelper(base->children[1], invalidNodes);
            }
    }

void fe::aabbTree::insertNode(node *base, node **parent)
    {
    }

void fe::aabbTree::removeNode(node *base)
    {
    }

void fe::aabbTree::computePairsHelper(node *n0, node *n1)
    {
    }

void fe::aabbTree::clearChildrenCrossFlagHelper(node *base)
    {
    }

void fe::aabbTree::crossChildren(node *base)
    {
    }

fe::aabbTree::aabbTree() : m_root(nullptr), m_margin(0.2f)
    {
    }

void fe::aabbTree::add(fe::collider *collider)
    {
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
    }

void fe::aabbTree::update(float dt)
    {
        if (m_root)
            {
                if (m_root->isLeaf())
                    {
                        m_root->updateAABB(m_margin);
                    }
                else
                    {
                        m_invalidNodes.clear();
                        updateNodeHelper(m_root, m_invalidNodes);

                        for (node *base : m_invalidNodes)
                            {
                                node *parent = base->parent;
                                node *sibling = base->getSibling();
                                node **parentLink = parent->parent ? (parent == parent->parent->children[0] ? &parent->parent->children[0] : &parent->parent->children[1]) : &m_root;

                                sibling->parent = parent->parent ? parent->parent : nullptr;

                                *parentLink = sibling;
                                delete parent;

                                base->updateAABB(m_margin);
                                insertNode(base, &m_root);
                            }
                    }
                m_invalidNodes.clear();
            }
    }

const std::list<std::pair<fe::collider*, fe::collider*>> fe::aabbTree::computeColliderPairs()
    {
        return std::list<std::pair<fe::collider*, fe::collider*>>();
    }

fe::collider *fe::aabbTree::colliderAtPoint(float x, float y)
    {
        return nullptr;
    }

std::pair<unsigned int, fe::collider*[FE_MAX_GAME_OBJECTS]> fe::aabbTree::collidesWithAABB(fe::collider &aabb)
    {
        return std::pair<unsigned int, fe::collider*[FE_MAX_GAME_OBJECTS]>();
    }

fe::raycastResult fe::aabbTree::raycast(float x, float y, float dirX, float dirY)
    {
        return fe::raycastResult();
    }
