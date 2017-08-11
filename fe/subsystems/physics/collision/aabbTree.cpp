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
                FE_ENGINE_PROFILE("aabb_tree", "update_aabb");
                fe::collider *collider = baseNode->m_userData;
                baseNode->m_fatAABB.m_positionX = collider->m_aabb.m_positionX - m_fatness;
                baseNode->m_fatAABB.m_positionY = collider->m_aabb.m_positionY - m_fatness;
                baseNode->m_fatAABB.m_sizeX = collider->m_aabb.m_sizeX + m_fatness + m_fatness;
                baseNode->m_fatAABB.m_sizeY = collider->m_aabb.m_sizeY + m_fatness + m_fatness;
                FE_END_PROFILE;
            }
        else
            {
                FE_ENGINE_PROFILE("aabb_tree", "parent_update_aabb");
                baseNode->m_fatAABB = baseNode->m_leftChild->m_fatAABB.merge(baseNode->m_rightChild->m_fatAABB);
                FE_END_PROFILE;
            }
    }

void fe::aabbTree::checkMovedColliders(node *baseNode)
    {
        if (baseNode->isLeaf())
            {
                FE_ENGINE_PROFILE("aabb_tree", "moved_collider_check_aabb");
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
        FE_ENGINE_PROFILE("aabb_tree", "collider_insert");
        node *currentParent = parent;
        while (!currentParent->isLeaf())
            {
                // Cost Calculation taken from Box2d: https://github.com/erincatto/Box2D/blob/master/Box2D/Box2D/Collision/b2DynamicTree.cpp

                float area = baseNode->m_fatAABB.perimeter();

                fe::AABB combinedAABB = currentParent->m_fatAABB.merge(baseNode->m_fatAABB);
                float combinedArea = combinedAABB.perimeter();
                float cost = 2.f * combinedArea;

                float inheritCost = 2.f * (combinedArea - area);
                float cost1 = 0.f;
                float cost2 = 0.f;
                if (currentParent->m_leftChild->isLeaf())
                    {
                        cost1 = baseNode->m_fatAABB.merge(currentParent->m_leftChild->m_fatAABB).perimeter() + inheritCost;
                    }
                else
                    {
                        float oldArea = currentParent->m_leftChild->m_fatAABB.perimeter();
                        float newArea = baseNode->m_fatAABB.merge(currentParent->m_leftChild->m_fatAABB).perimeter();
                        cost1 = (newArea - oldArea) + inheritCost;
                    }

                if (currentParent->m_rightChild->isLeaf())
                    {
                        cost2 = baseNode->m_fatAABB.merge(currentParent->m_rightChild->m_fatAABB).perimeter() + inheritCost;
                    }
                else
                    {
                        float oldArea = currentParent->m_rightChild->m_fatAABB.perimeter();
                        float newArea = baseNode->m_fatAABB.merge(currentParent->m_rightChild->m_fatAABB).perimeter();
                        cost2 = (newArea - oldArea) + inheritCost;
                    }

                currentParent = (cost1 < cost2 ? currentParent->m_leftChild : currentParent->m_rightChild);
            }

        node *newParent = m_nodes.alloc();
        newParent->m_parent = currentParent->m_parent; FE_ASSERT(newParent->m_parent != newParent, "NewParent's parent cannot be newParent");
        if (newParent->m_parent)
            {
                if (newParent->m_parent->m_leftChild == currentParent)
                    {
                        newParent->m_parent->m_leftChild = newParent;
                    }
                else
                    {
                        newParent->m_parent->m_rightChild = newParent;
                    }
            }

        newParent->m_leftChild = baseNode;
        newParent->m_rightChild = currentParent;

        newParent->m_leftChild->m_parent = newParent; FE_ASSERT(newParent->m_leftChild->m_parent != newParent->m_leftChild, "NewParent->m_leftChild cannot be itself");
        newParent->m_rightChild->m_parent = newParent; FE_ASSERT(newParent->m_rightChild->m_parent != newParent->m_rightChild, "NewParent->m_rightChild cannot be itself");

        FE_ASSERT(currentParent->m_parent != currentParent, "CurrentParents's parent cannot be CurrentParent");
        FE_ASSERT(baseNode->m_parent != baseNode, "BaseNode's parent cannot be BaseNode");

        FE_END_PROFILE;
    }

void fe::aabbTree::balance(node **baseNode)
    {
        FE_ENGINE_PROFILE("aabb_tree", "balance_tree");
        if ((*baseNode)->isLeaf())
            {
                return;
            }

        int balance = (*baseNode)->m_rightChild->m_height - (*baseNode)->m_leftChild->m_height;
        if (balance > 1)
            {
                balance = (*baseNode)->m_rightChild->isLeaf() ? 0 : (*baseNode)->m_rightChild->m_rightChild->m_height - (*baseNode)->m_rightChild->m_leftChild->m_height;
                // Single Left Rotation
                node *a = (*baseNode);
                node *b = a->m_rightChild;
                node *c = b->m_rightChild;
                node *d = b->m_leftChild;
                node *e = a->m_leftChild;

                b->m_leftChild = a;
                a->m_rightChild = d;

                node *aParent = a->m_parent;
                a->m_parent = b;
                b->m_parent = aParent;

                b->m_leftChild->m_parent = b; FE_ASSERT(b->m_parent != b, "B cannot be B's Parent");
                b->m_rightChild->m_parent = b; FE_ASSERT(b->m_parent != b, "B cannot be B's Parent");
                a->m_rightChild->m_parent = a; FE_ASSERT(a->m_parent != a, "A cannot be A's Parent");
                a->m_leftChild->m_parent = a; FE_ASSERT(a->m_parent != a, "A cannot be A's Parent");

                a->m_height = std::max(a->m_leftChild->m_height, a->m_rightChild->m_height) + 1;
                b->m_height = std::max(b->m_leftChild->m_height, b->m_rightChild->m_height) + 1;

                if (b->m_parent)
                    {
                        b->m_parent->m_rightChild = b;
                    }
                *baseNode = b;
            }
        else if (balance < -1)
            {
                balance = (*baseNode)->m_leftChild->isLeaf() ? 0 : (*baseNode)->m_leftChild->m_rightChild->m_height - (*baseNode)->m_leftChild->m_leftChild->m_height;
                // Single Right Rotation
                node *a = (*baseNode)->m_leftChild;
                node *b = (*baseNode);
                node *c = b->m_rightChild;
                node *d = a->m_rightChild;
                node *e = a->m_leftChild;

                a->m_rightChild = b;
                b->m_leftChild = d;

                node *bParent = b->m_parent;
                b->m_parent = a;
                a->m_parent = bParent;

                b->m_leftChild->m_parent = b; FE_ASSERT(b->m_parent != b, "B cannot be B's Parent");
                b->m_rightChild->m_parent = b; FE_ASSERT(b->m_parent != b, "B cannot be B's Parent");

                a->m_rightChild->m_parent = a; FE_ASSERT(a->m_parent != a, "A cannot be A's Parent");
                a->m_leftChild->m_parent = a; FE_ASSERT(a->m_parent != a, "A cannot be A's Parent");

                a->m_height = std::max(a->m_leftChild->m_height, a->m_rightChild->m_height) + 1;
                b->m_height = std::max(b->m_leftChild->m_height, b->m_rightChild->m_height) + 1;

                if (a->m_parent) 
                    {
                        a->m_parent->m_leftChild = a;
                    }
                *baseNode = a;
            }
        FE_END_PROFILE;
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
        FE_ENGINE_PROFILE("aabb_tree", "add_collider");
        if (m_base)
            {
                node *newNode = m_nodes.alloc();
                newNode->m_height = 0;
                newNode->m_userData = collider;
                insert(newNode, m_base);

                newNode = newNode->m_parent;
                while (newNode)
                    {
                        balance(&newNode);
                        newNode->m_height = std::max(newNode->m_leftChild->m_height, newNode->m_rightChild->m_height) + 1;
                        updateAABB(newNode);

                        if (!newNode->m_parent) m_base = newNode;
                        newNode = newNode->m_parent;
                    }
            }
        else
            {
                m_base = m_nodes.alloc();
                m_base->m_height = 0;
                m_base->m_userData = collider;
            }
        FE_END_PROFILE;
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
        balance(&m_base);
    }

void fe::aabbTree::update()
    {
        FE_ENGINE_PROFILE("aabb_tree", "update");
        m_movedNodesIndex = 0;
        if (m_base)
            {
                if (m_base->isLeaf())
                    {
                        m_base->m_fatAABB.m_positionX = m_base->m_userData->m_aabb.m_positionX - m_fatness;
                        m_base->m_fatAABB.m_positionY = m_base->m_userData->m_aabb.m_positionY - m_fatness;
                        m_base->m_fatAABB.m_sizeX = m_base->m_userData->m_aabb.m_sizeX + m_fatness + m_fatness;
                        m_base->m_fatAABB.m_sizeY = m_base->m_userData->m_aabb.m_sizeY + m_fatness + m_fatness;
                    }
                else
                    {
                        FE_ENGINE_PROFILE("aabb_tree", "check_moved_colliders");
                        checkMovedColliders(m_base);
                        FE_END_PROFILE;
                        for (unsigned int i = 0; i < m_movedNodesIndex; i++)
                            {
                                FE_ENGINE_PROFILE("aabb_tree", "invalid_node_update");
                                node *baseNode = m_movedNodes[i];
                                if (baseNode->m_parent == nullptr) continue;
                                node *sibling = baseNode->getSibling();
                                node *parent = baseNode->m_parent;
                                
                                if (sibling->isLeaf()) 
                                    {
                                        parent->m_userData = sibling->m_userData;
                                        parent->m_leftChild = nullptr;
                                        parent->m_rightChild = nullptr;
                                    }
                                else
                                    {
                                        parent->m_leftChild = sibling->m_leftChild;
                                        parent->m_rightChild = sibling->m_rightChild;
                                        parent->m_leftChild->m_parent = parent;
                                        parent->m_rightChild->m_parent = parent;

                                        FE_ASSERT(parent->m_leftChild != parent, "Parents Left child = Parent");
                                        FE_ASSERT(parent->m_rightChild != parent, "Parents Right child = Parent");
                                    }
                                parent->m_height = 0;
                                m_nodes.free(sibling);

                                updateAABB(baseNode);
                                insert(baseNode, m_base);
                                FE_ENGINE_PROFILE("aabb_tree", "balance_iter");
                                node *testNode = baseNode->m_parent;
                                while (testNode)
                                    {
                                        balance(&testNode);
                                        testNode->m_height = std::max(testNode->m_leftChild->m_height, testNode->m_rightChild->m_height) + 1;
                                        updateAABB(testNode);

                                        if (!testNode->m_parent) m_base = testNode;
                                        testNode = testNode->m_parent;
                                    }
                                FE_END_PROFILE;
                                FE_END_PROFILE;
                            }
                    }

                if (m_debug)
                    {
                        FE_ENGINE_PROFILE("aabb_tree", "debug_draw");
                        debugDrawAABB(m_base);
                        FE_END_PROFILE;
                    }
            }
        FE_END_PROFILE;
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