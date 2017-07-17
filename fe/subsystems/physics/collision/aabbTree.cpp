#include "aabbTree.hpp"
#include "collisionBody.hpp"
#include <algorithm>

#include "../../../debug/debugDraw.hpp"
#include "../../../debug/profiler.hpp"


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

void fe::aabbTree::node::setLeaf(fe::collider *data)
    {
        this->data = data;
        data->m_userData = this;

        children[0] = nullptr;
        children[1] = nullptr;
    }

void fe::aabbTree::node::updateAABB(float margin)
    {
        if (isLeaf())
            {
                aabb.m_positionX = data->m_aabb.m_positionX - margin;
                aabb.m_positionY = data->m_aabb.m_positionY - margin;

                aabb.m_sizeX = data->m_aabb.m_sizeX + margin + margin;
                aabb.m_sizeY = data->m_aabb.m_sizeY + margin + margin;
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

void fe::aabbTree::updateNodeHelper(node *base, node *invalidNodes[FE_MAX_GAME_OBJECTS])
    {
        if (base->isLeaf())
            {
                if (!base->aabb.contains(&base->data->m_aabb))
                    {
                        invalidNodes[m_invalidNodesIndex++] = base;
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
        if (!n0 || !n1)
            {
                return;
            }

        if (n0->isLeaf())
            {
                // 2 Leaves, check proxies instead of fat AABBs
                if (n1->isLeaf() && n0->data->m_aabb.intersects(&n1->data->m_aabb))
                    {
                        m_pairs[m_pairsIndex].first =   n0->data;
                        m_pairs[m_pairsIndex].second =  n1->data;
                        m_pairsIndex++;
                    }
                else // 1 Branch / 1 Leaf, 2 cross checks
                    {
                        crossChildren(n1);
                        computePairsHelper(n0, n1->children[0]);
                        computePairsHelper(n0, n1->children[1]);
                    }
            }
        else if (n1->isLeaf())
            {
                crossChildren(n0);
                computePairsHelper(n0->children[0], n1);
                computePairsHelper(n0->children[1], n1);
            }
        else
            {
                crossChildren(n0);
                crossChildren(n1);
                computePairsHelper(n0->children[0], n1->children[0]);
                computePairsHelper(n0->children[0], n1->children[1]);
                computePairsHelper(n0->children[1], n1->children[0]);
                computePairsHelper(n0->children[1], n1->children[1]);
            }
    }

void fe::aabbTree::clearChildrenCrossFlagHelper(node *base)
    {
        base->childrenCrossed = false;
        if (!base->isLeaf())
            {
                clearChildrenCrossFlagHelper(base->children[0]);
                clearChildrenCrossFlagHelper(base->children[1]);
            }
    }

void fe::aabbTree::crossChildren(node *base)
    {
        if (!base->childrenCrossed)
            {
                computePairsHelper(base->children[0], base->children[1]);
                base->childrenCrossed = true;
            }
    }

void fe::aabbTree::drawAABB(node *base)
    {
        if (base->isLeaf())
            {
                FE_DEBUG_DRAW_SQUARE(base->aabb.m_sizeX, base->aabb.m_sizeY, base->aabb.m_positionX, base->aabb.m_positionY, sf::Color::Red);
            }
        else
            {
                drawAABB(base->children[0]);
                drawAABB(base->children[1]);
            }
    }

fe::aabbTree::aabbTree() : m_root(nullptr), m_margin(12.0f), m_pairsIndex(0), m_invalidNodesIndex(0)
    {
    }

void fe::aabbTree::add(fe::collider *collider)
    {
        if (m_root)
            {
                node *base = new node();
                base->setLeaf(collider);
                base->updateAABB(m_margin);
                insertNode(base, &m_root);
            }
        else
            {
                m_root = new node();
                m_root->setLeaf(collider);
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
        m_invalidNodesIndex = 0;
        if (m_root)
            {
                if (m_root->isLeaf())
                    {
                        FE_PROFILE("aabb_tree", "root_aabb_update");
                        m_root->updateAABB(m_margin);
                        FE_END_PROFILE;
                    }
                else
                    {
                        FE_PROFILE("aabb_tree", "update_node_helper");
                        updateNodeHelper(m_root, m_invalidNodes);
                        FE_END_PROFILE;

                        FE_PROFILE("aabb_tree", "invalid_node_computation");
                        for (unsigned int i = 0; i < m_invalidNodesIndex; i++)
                            {
                                node *base = m_invalidNodes[i];

                                node *parent = base->parent;
                                node *sibling = base->getSibling();
                                node **parentLink = parent->parent ? (parent == parent->parent->children[0] ? &parent->parent->children[0] : &parent->parent->children[1]) : &m_root;

                                sibling->parent = parent->parent ? parent->parent : nullptr;

                                *parentLink = sibling;
                                delete parent;

                                base->updateAABB(m_margin);
                                insertNode(base, &m_root);
                            }
                        FE_END_PROFILE;
                    }

                FE_PROFILE("aabb_tree", "debug_draw");
                if (m_debug)
                    {
                        drawAABB(m_root);
                    }
                FE_END_PROFILE;
            }
    }

const std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int> fe::aabbTree::computeColliderPairs()
    {
        m_pairsIndex = 0;
        if (!m_root || m_root->isLeaf())
            {
                return std::make_pair(m_pairs, 0);
            }

        clearChildrenCrossFlagHelper(m_root);

        computePairsHelper(m_root->children[0], m_root->children[1]);

        return std::make_pair(m_pairs, m_pairsIndex);
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
