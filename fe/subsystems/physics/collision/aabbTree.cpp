#include "aabbTree.hpp"
#include "collisionBody.hpp"
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

void fe::aabbTree::node::setLeaf(fe::AABB *data, void **userData)
    {
        this->data = data;
        (*userData) = this;

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
                aabb = children[0]->aabb.merge(&children[1]->aabb);
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
        node *p = *parent;
        if (p->isLeaf())
            {
                node *newParent = new node();
                newParent->parent = p->parent;
                newParent->setBranch(base, p);
                *parent = newParent;
            }
        else
            {
                fe::AABB &aabb0 = p->children[0]->aabb;
                fe::AABB &aabb1 = p->children[1]->aabb;

                float volumeDiff0 = aabb0.merge(&base->aabb).volume() - aabb0.volume();
                float volumeDiff1 = aabb1.merge(&base->aabb).volume() - aabb1.volume();

                if (volumeDiff0 < volumeDiff1)
                    {
                        insertNode(base, &p->children[0]);
                    }
                else
                    {
                        insertNode(base, &p->children[1]);
                    }
            }

        (*parent)->updateAABB(m_margin);
    }

void fe::aabbTree::removeNode(node *base)
    {
        node *parent = base->parent;
        if (parent)
            {
                node *sibling = base->getSibling();
                if (parent->parent)
                    {
                        sibling->parent = parent->parent;
                        (parent == parent->parent->children[0] ? parent->parent->children[0] : parent->parent->children[1]) = sibling;
                    }
                else
                    {
                        node *sibling = base->getSibling();
                        m_root = sibling;
                        sibling->parent = nullptr;
                    }
                delete parent;
            }
        else
            {
                m_root = nullptr;
            }
        delete base;
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
        if (m_root)
            {
                node *base = new node();
                base->setLeaf(&collider->m_aabb, &collider->m_userData);
                base->updateAABB(m_margin);
                insertNode(base, &m_root);
            }
        else
            {
                m_root = new node();
                m_root->setLeaf(&collider->m_aabb, &collider->m_userData);
                m_root->updateAABB(m_margin);
            }
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
        node *base = static_cast<node*>(collider->m_userData);

        base->data = nullptr;
        collider->m_userData = nullptr;

        removeNode(base);
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
