#include "aabbTree.hpp"
#include "collisionBody.hpp"

#include "../../../debug/debugDraw.hpp"
#include "../../../debug/profiler.hpp"

void fe::aabbTree::debugDrawAABB(node *base)
    {
        if (!base) return;
        debugDrawAABB(base->m_leftChild);
        debugDrawAABB(base->m_rightChild);

        FE_DEBUG_DRAW_SQUARE(base->m_fatAABB.m_sizeX, base->m_fatAABB.m_sizeY, base->m_fatAABB.m_positionX, base->m_fatAABB.m_positionY, sf::Color::Red);
    }

void fe::aabbTree::updateAABB(node *baseNode)
    {
        if (baseNode->isLeaf())
            {
                FE_PROFILE("aabb_tree", "update_aabb");
                fe::collider *collider = baseNode->m_userData;
                baseNode->m_fatAABB.m_positionX = collider->m_aabb.m_positionX - m_fatness;
                baseNode->m_fatAABB.m_positionY = collider->m_aabb.m_positionY - m_fatness;
                baseNode->m_fatAABB.m_sizeX = collider->m_aabb.m_sizeX + m_fatness;
                baseNode->m_fatAABB.m_sizeY = collider->m_aabb.m_sizeY + m_fatness;
                FE_END_PROFILE;
            }
        else
            {
                FE_PROFILE("aabb_tree", "parent_update_aabb");
                updateAABB(baseNode->m_leftChild);
                updateAABB(baseNode->m_rightChild);

                baseNode->m_fatAABB = baseNode->m_leftChild->m_fatAABB.merge(baseNode->m_rightChild->m_fatAABB);
                FE_END_PROFILE;
            }
    }

void fe::aabbTree::checkMovedColliders(node *baseNode)
    {
        if (baseNode->isLeaf())
            {
                FE_PROFILE("aabb_tree", "moved_collider_check_aabb");
                if (!baseNode->m_fatAABB.contains(baseNode->m_userData->m_aabb))
                    {
                        m_movedNodes[m_movedNodesIndex++] = baseNode;
                    }
                FE_END_PROFILE;
            }
        else
            {
                checkMovedColliders(baseNode->m_leftChild);
                checkMovedColliders(baseNode->m_rightChild);
            }
    }

void fe::aabbTree::insert(node *baseNode, node *parent)
    {
        FE_PROFILE("aabb_tree", "collider_insert");
        if (parent->isLeaf())
            {
                node *newNode = m_nodes.alloc();
                newNode->m_userData = parent->m_userData;
                parent->m_userData = nullptr;

                parent->m_leftChild = newNode;
                parent->m_rightChild = baseNode;

                newNode->m_parent = parent;
                baseNode->m_parent = parent;
            }
        else
            {
                // Cost Calculation taken from Box2d: https://github.com/erincatto/Box2D/blob/master/Box2D/Box2D/Collision/b2DynamicTree.cpp

                float area = baseNode->m_fatAABB.perimeter();

                fe::AABB combinedAABB = parent->m_fatAABB.merge(baseNode->m_fatAABB);
                float combinedArea = combinedAABB.perimeter();
                float cost = 2.f * combinedArea;

                float inheritCost = 2.f * (combinedArea - area);
                float cost1 = 0.f;
                float cost2 = 0.f;
                if (parent->m_leftChild->isLeaf())
                    {
                        cost1 = baseNode->m_fatAABB.merge(parent->m_leftChild->m_fatAABB).perimeter() + inheritCost;
                    }
                else
                    {
                        float oldArea = parent->m_leftChild->m_fatAABB.perimeter();
                        float newArea = baseNode->m_fatAABB.merge(parent->m_leftChild->m_fatAABB).perimeter();
                        cost1 = (newArea - oldArea) + inheritCost;
                    }

                if (parent->m_rightChild->isLeaf())
                    {
                        cost2 = baseNode->m_fatAABB.merge(parent->m_rightChild->m_fatAABB).perimeter() + inheritCost;
                    }
                else
                    {
                        float oldArea = parent->m_rightChild->m_fatAABB.perimeter();
                        float newArea = baseNode->m_fatAABB.merge(parent->m_rightChild->m_fatAABB).perimeter();
                        cost2 = (newArea - oldArea) + inheritCost;
                    }

                insert(baseNode, cost1 < cost2 ? parent->m_leftChild : parent->m_rightChild);
            }
        FE_END_PROFILE;

        updateAABB(parent);
    }


fe::aabbTree::aabbTree() : m_fatness(5.f), m_base(nullptr)
    {
    }

void fe::aabbTree::startUp()
    {
        m_nodes.startUp(FE_MAX_GAME_OBJECTS);
    }

void fe::aabbTree::add(fe::collider *collider)
    {
        if (m_base)
            {
                node *newNode = m_nodes.alloc();
                newNode->m_userData = collider;
                insert(newNode, m_base);
            }
        else
            {
                m_base = m_nodes.alloc();
                m_base->m_userData = collider;
            }
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
        
    }

void fe::aabbTree::update()
    {
        m_movedNodesIndex = 0;
        if (m_base)
            {
                if (m_base->isLeaf())
                    {
                        updateAABB(m_base);
                    }
                else
                    {
                        FE_PROFILE("aabb_tree", "check_moved_colliders");
                        checkMovedColliders(m_base);
                        FE_END_PROFILE;
                        for (unsigned int i = 0; i < m_movedNodesIndex; i++)
                            {
                                FE_PROFILE("aabb_tree", "invalid_node_update");
                                node *baseNode = m_movedNodes[i];
                                node *sibling = baseNode->getSibling();
                                node *parent = baseNode->m_parent;
                                
                                if (sibling->m_userData) 
                                    {
                                        parent->m_userData = sibling->m_userData;
                                        parent->m_leftChild = nullptr;
                                        parent->m_rightChild = nullptr;
                                    }
                                else
                                    {
                                        parent->m_leftChild = sibling->m_leftChild;
                                        parent->m_rightChild = sibling->m_rightChild;
                                        sibling->m_leftChild->m_parent = parent;
                                        sibling->m_rightChild->m_parent = parent;
                                    }

                                m_nodes.free(sibling);

                                updateAABB(baseNode);
                                insert(baseNode, m_base);
                                FE_END_PROFILE;
                            }
                    }

                if (m_debug)
                    {
                        FE_PROFILE("aabb_tree", "debug_draw");
                        debugDrawAABB(m_base);
                        FE_END_PROFILE;
                    }
            }
    }

const std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int> fe::aabbTree::computeColliderPairs()
    {
        return std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int>();
    }

fe::collider *fe::aabbTree::colliderAtPoint(float x, float y)
    {
        return nullptr;
    }

std::pair<fe::collider*[FE_MAX_GAME_OBJECTS], unsigned int> fe::aabbTree::collidesWithAABB(fe::collider &aabb)
    {
        return std::pair<fe::collider*[FE_MAX_GAME_OBJECTS], unsigned int>();
    }

fe::raycastResult fe::aabbTree::raycast(float x, float y, float dirX, float dirY)
    {
        return fe::raycastResult();
    }
