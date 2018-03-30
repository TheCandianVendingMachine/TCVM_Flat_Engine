#include "aabbTree.hpp"
#include "collisionBody.hpp"

#include "../../../debug/debugDraw.hpp"
#include "../../../debug/profiler.hpp"

#include <stack>

void fe::aabbTree::debugDrawAABB(int node)
    {
        if (node == treeNode::Null) return;
        if (!m_nodes[node].isLeaf())
            {
                m_nodes[node].m_fatAABB = (m_nodes[m_nodes[node].m_left].m_fatAABB.merge(m_nodes[m_nodes[node].m_right].m_fatAABB));

                debugDrawAABB(m_nodes[node].m_left);
                debugDrawAABB(m_nodes[node].m_right);
            }

        fe::AABB &currentNodeAABB = m_nodes[node].m_fatAABB;
        FE_DEBUG_DRAW_SQUARE(   static_cast<int>(std::ceilf(currentNodeAABB.m_sizeX)), static_cast<int>(std::ceilf(currentNodeAABB.m_sizeY)),
                                static_cast<int>(std::ceilf(currentNodeAABB.m_globalPositionX)), static_cast<int>(std::ceilf(currentNodeAABB.m_globalPositionY)),
                                sf::Color::Red);
    }

void fe::aabbTree::freeNode(int node)
    {
        FE_PROFILE("aabb_tree", "free_node");
        m_nodes[node].m_next = m_freeList;
        m_nodes[node].m_height = -1;
        m_freeList = node;
        FE_END_PROFILE;
    }

int fe::aabbTree::allocateNode()
    {
        FE_PROFILE("aabb_tree", "allocate_node");
        FE_ASSERT(m_freeList != treeNode::Null, "Free-List of nodes is empty");

        int nodeId = m_freeList;
        m_freeList = m_nodes[nodeId].m_next;
        m_nodes[nodeId].m_parent = treeNode::Null;
        m_nodes[nodeId].m_left = treeNode::Null;
        m_nodes[nodeId].m_right = treeNode::Null;
        m_nodes[nodeId].m_height = 0;
        m_nodes[nodeId].m_userData = nullptr;
        return nodeId;
        FE_END_PROFILE;
    }

void fe::aabbTree::insert(int node)
    {
        FE_PROFILE("aabb_tree", "insert_index");
        if (m_base == treeNode::Null)
            {
                m_base = node;
                m_nodes[m_base].m_parent = treeNode::Null;
                return;
            }

        fe::AABB leafAABB = m_nodes[node].m_fatAABB;
        int index = m_base;
        while (!m_nodes[index].isLeaf())
            {
                int left = m_nodes[index].m_left;
                int right = m_nodes[index].m_right;

                float area = m_nodes[index].m_fatAABB.perimeter();

                fe::AABB combinedAABB = m_nodes[index].m_fatAABB.merge(leafAABB);
                float combinedArea = combinedAABB.perimeter();

                float cost = 2.f * combinedArea;

                float inheritCost = 2.f * (combinedArea - area);

                float cost1 = 0.f;
                float cost2 = 0.f;

                if (m_nodes[left].isLeaf())
                    {
                        cost1 = leafAABB.merge(m_nodes[left].m_fatAABB).perimeter() + inheritCost;
                    }
                else
                    {
                        float newArea = leafAABB.merge(m_nodes[left].m_fatAABB).perimeter() + inheritCost;
                        float oldArea = m_nodes[left].m_fatAABB.perimeter();
                        cost1 = (newArea - oldArea) + inheritCost;
                    }

                if (m_nodes[right].isLeaf())
                    {
                        cost2 = leafAABB.merge(m_nodes[right].m_fatAABB).perimeter() + inheritCost;
                    }
                else
                    {
                        float newArea = leafAABB.merge(m_nodes[right].m_fatAABB).perimeter() + inheritCost;
                        float oldArea = m_nodes[right].m_fatAABB.perimeter();
                        cost2 = newArea - oldArea + inheritCost;
                    }

                if (cost < cost1 && cost < cost2)
                    {
                        break;
                    }

                if (cost1 < cost2)
                    {
                        index = left;
                    }
                else
                    {
                        index = right;
                    }
            }

        int sibling = index;

        int oldParent = m_nodes[sibling].m_parent;
        int newParent = allocateNode();
        m_nodes[newParent].m_parent = oldParent;
        m_nodes[newParent].m_userData = nullptr;
        m_nodes[newParent].m_fatAABB = leafAABB.merge(m_nodes[sibling].m_fatAABB);
        m_nodes[newParent].m_height = m_nodes[sibling].m_height + 1;

        if (oldParent != treeNode::Null)
            {
                if (m_nodes[oldParent].m_left == sibling)
                    {
                        m_nodes[oldParent].m_left = newParent;
                    }
                else
                    {
                        m_nodes[oldParent].m_right = newParent;
                    }

                m_nodes[newParent].m_left = sibling;
                m_nodes[newParent].m_right = node;
                m_nodes[sibling].m_parent = newParent;
                m_nodes[node].m_parent = newParent;
            }
        else
            {
                m_nodes[newParent].m_left = sibling;
                m_nodes[newParent].m_right = node;
                m_nodes[sibling].m_parent = newParent;
                m_nodes[node].m_parent = newParent;
                m_base = newParent;
            }

        index = m_nodes[node].m_parent;
        while (index != treeNode::Null)
            {
                index = balance(index);

                int leftChild = m_nodes[index].m_left;
                int rightChild = m_nodes[index].m_right;

                m_nodes[index].m_height = std::max(m_nodes[leftChild].m_height, m_nodes[rightChild].m_height) + 1;
                m_nodes[index].m_fatAABB = m_nodes[leftChild].m_fatAABB.merge(m_nodes[rightChild].m_fatAABB);

                index = m_nodes[index].m_parent;
            }
        FE_END_PROFILE;
    }

void fe::aabbTree::remove(int node)
    {
        FE_ENGINE_PROFILE("aabb_tree", "remove_node_index");
        if (node == m_base)
            {
                m_base = treeNode::Null;
                return;
            }

        int parent = m_nodes[node].m_parent;
        int grandParent = m_nodes[parent].m_parent;
        int sibling = 0;
        if (m_nodes[parent].m_left == node)
            {
                sibling = m_nodes[parent].m_right;
            }
        else
            {
                sibling = m_nodes[parent].m_left;
            }

        if (grandParent != treeNode::Null)
            {
                if (m_nodes[grandParent].m_left == parent)
                    {
                        m_nodes[grandParent].m_left = sibling;
                    }
                else
                    {
                        m_nodes[grandParent].m_right = sibling;
                    }
                m_nodes[sibling].m_parent = grandParent;
                freeNode(parent);

                int index = grandParent;
                while (index != treeNode::Null)
                    {
                        index = balance(index);

                        int childLeft = m_nodes[index].m_left;
                        int childRight = m_nodes[index].m_right;

                        m_nodes[index].m_fatAABB = m_nodes[childLeft].m_fatAABB.merge(m_nodes[childRight].m_fatAABB);
                        m_nodes[index].m_height = std::max(m_nodes[childLeft].m_height, m_nodes[childRight].m_height) + 1;

                        index = m_nodes[index].m_parent;
                    }
            }
        else
            {
                m_base = sibling;
                m_nodes[sibling].m_parent = treeNode::Null;
                freeNode(parent);
            }
        FE_END_PROFILE;
    }

int fe::aabbTree::balance(int node)
    {
        FE_PROFILE("aabb_tree", "balance_tree");
        treeNode *a = &m_nodes[node];
        if (a->isLeaf() || a->m_height < 2)
            {
                return node;
            }

        int iB = a->m_left;
        int iC = a->m_right;

        treeNode *b = &m_nodes[iB];
        treeNode *c = &m_nodes[iC];

        int balance = c->m_height - b->m_height;

        if (balance > 1)
            {
                int iF = c->m_left;
                int iG = c->m_right;
                treeNode *f = &m_nodes[iF];
                treeNode *g = &m_nodes[iG];

                c->m_left = node;
                c->m_parent = a->m_parent;
                a->m_parent = iC;

                if (c->m_parent != treeNode::Null)
                    {
                        if (m_nodes[c->m_parent].m_left == node)
                            {
                                m_nodes[c->m_parent].m_left = iC;
                            }
                        else
                            {
                                FE_ASSERT(m_nodes[c->m_parent].m_right == node, "C's parents right child cannot be the node");
                                m_nodes[c->m_parent].m_right = iC;
                            }
                    }
                else
                    {
                        m_base = iC;
                    }

                if (f->m_height > g->m_height)
                    {
                        c->m_right = iF;
                        a->m_right = iG;
                        g->m_parent = node;
                        a->m_fatAABB = b->m_fatAABB.merge(g->m_fatAABB);
                        c->m_fatAABB = a->m_fatAABB.merge(f->m_fatAABB);

                        a->m_height = std::max(b->m_height, g->m_height) + 1;
                        c->m_height = std::max(a->m_height, f->m_height) + 1;
                    }
                else
                    {
                        c->m_right = iG;
                        a->m_right = iF;
                        f->m_parent = node;
                        a->m_fatAABB = b->m_fatAABB.merge(f->m_fatAABB);
                        c->m_fatAABB = a->m_fatAABB.merge(g->m_fatAABB);

                        a->m_height = std::max(b->m_height, f->m_height) + 1;
                        c->m_height = std::max(a->m_height, g->m_height) + 1;
                    }

                return iC;
            }
        
        if (balance < -1)
            {
                int iD = b->m_left;
                int iE = b->m_right;
                treeNode *d = &m_nodes[iD];
                treeNode *e = &m_nodes[iE];

                b->m_left = node;
                b->m_parent = a->m_parent;
                a->m_parent = iB;

                if (b->m_parent != treeNode::Null)
                    {
                        if (m_nodes[b->m_parent].m_left == node)
                            {
                                m_nodes[b->m_parent].m_left = iB;
                            }
                        else
                            {
                                FE_ASSERT(m_nodes[b->m_parent].m_right == node, "B's parents right child cannot be the node");
                                m_nodes[b->m_parent].m_right = iB;
                            }
                    }
                else
                    {
                        m_base = iB;
                    }

                if (d->m_height > e->m_height)
                    {
                        b->m_right = iD;
                        a->m_left = iE;
                        e->m_parent = node;
                        a->m_fatAABB = c->m_fatAABB.merge(e->m_fatAABB);
                        b->m_fatAABB = a->m_fatAABB.merge(d->m_fatAABB);

                        a->m_height = std::max(c->m_height, e->m_height) + 1;
                        b->m_height = std::max(a->m_height, d->m_height) + 1;
                    }
                else
                    {
                        b->m_right = iE;
                        a->m_left = iD;
                        d->m_parent = node;
                        a->m_fatAABB = c->m_fatAABB.merge(d->m_fatAABB);
                        b->m_fatAABB = a->m_fatAABB.merge(e->m_fatAABB);
                        a->m_height = std::max(c->m_height, d->m_height) + 1;
                        b->m_height = std::max(a->m_height, e->m_height) + 1;
                    }

                return iB;
            }

        return node;
        FE_END_PROFILE;
    }

void *fe::aabbTree::pointCollideBranch(float x, float y, int branch) const
    {
        if (branch < 0)
            {
                return nullptr;
            }
        if (m_nodes[branch].m_fatAABB.contains(x, y))
            {
                if (m_nodes[branch].isLeaf())
                    {
                        return (void*)(&m_nodes[branch]);
                    }
                else
                    {
                        treeNode *a = static_cast<treeNode*>(pointCollideBranch(x, y, m_nodes[branch].m_left));
                        if (a) return a;
                        return pointCollideBranch(x, y, m_nodes[branch].m_right);
                    }
            }
        return nullptr;
    }

void fe::aabbTree::AABBCollideBranch(fe::AABB &testAABB, std::function<void(void*)> callback, int branch) const
    {
        if (m_nodes[branch].m_fatAABB.intersects(testAABB))
            {
                if (m_nodes[branch].isLeaf())
                    {
                        callback(m_nodes[branch].m_userData);
                    }
                else
                    {
                        AABBCollideBranch(testAABB, callback, m_nodes[branch].m_left);
                        AABBCollideBranch(testAABB, callback, m_nodes[branch].m_right);
                    }
            }
    }

fe::aabbTree::aabbTree() : m_base(treeNode::Null), m_fatness(5.f)
    {
        for (unsigned int i = 0; i < m_nodeCapacity - 1; i++)
            {
                m_nodes[i].m_next = i + 1;
                m_nodes[i].m_height = -1;
                m_nodes[i].m_fatAABB.m_offsetX = 0.f;
                m_nodes[i].m_fatAABB.m_offsetY = 0.f;
                m_nodes[i].m_fatAABB.m_globalPositionX = 0.f;
                m_nodes[i].m_fatAABB.m_globalPositionX = 0.f;
                m_nodes[i].m_fatAABB.m_sizeX = 0.f;
                m_nodes[i].m_fatAABB.m_sizeY = 0.f;
            }
        m_nodes[m_nodeCapacity - 1].m_next = treeNode::Null;
        m_nodes[m_nodeCapacity - 1].m_height = -1;
        m_nodes[m_nodeCapacity - 1].m_fatAABB.m_offsetX = 0.f;
        m_nodes[m_nodeCapacity - 1].m_fatAABB.m_offsetY = 0.f;
        m_nodes[m_nodeCapacity - 1].m_fatAABB.m_globalPositionX = 0.f;
        m_nodes[m_nodeCapacity - 1].m_fatAABB.m_globalPositionX = 0.f;
        m_nodes[m_nodeCapacity - 1].m_fatAABB.m_sizeX = 0.f;
        m_nodes[m_nodeCapacity - 1].m_fatAABB.m_sizeY = 0.f;

        m_freeList = 0;
    }

void fe::aabbTree::startUp()
    {
        
    }

void fe::aabbTree::add(fe::collider *collider)
    {
        FE_ENGINE_PROFILE("aabb_tree", "add_collider");
        int colliderId = allocateNode();
        m_nodes[colliderId].m_userData = collider;
        collider->m_userData = (void*)(colliderId);
        m_nodes[colliderId].m_height = 0;
        
        treeNode *currentNode = &m_nodes[colliderId];
        currentNode->m_fatAABB.m_globalPositionX = collider->m_aabb.m_globalPositionX - m_fatness;
        currentNode->m_fatAABB.m_globalPositionY = collider->m_aabb.m_globalPositionY - m_fatness;
        currentNode->m_fatAABB.m_sizeX = collider->m_aabb.m_sizeX + m_fatness + m_fatness;
        currentNode->m_fatAABB.m_sizeY = collider->m_aabb.m_sizeY + m_fatness + m_fatness;

        insert(colliderId);
        FE_END_PROFILE;
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
        if (!collider) return;
        FE_PROFILE("aabb_tree", "remove_collider");
        remove(int(collider->m_userData));
        collider->m_userData = nullptr;
        FE_END_PROFILE;
    }

void fe::aabbTree::update(fe::collider *collider)
    {
        if (!collider->m_enabled) return;

        FE_PROFILE("aabb_tree", "update_collider");
        int nodeIndex = int(collider->m_userData);
        if (!m_nodes[nodeIndex].m_fatAABB.contains(collider->m_aabb))
            {
                remove(nodeIndex);
                treeNode *currentNode = &m_nodes[nodeIndex];
                currentNode->m_fatAABB.m_globalPositionX = collider->m_aabb.m_globalPositionX - m_fatness;
                currentNode->m_fatAABB.m_globalPositionY = collider->m_aabb.m_globalPositionY - m_fatness;
                currentNode->m_fatAABB.m_sizeX = collider->m_aabb.m_sizeX + m_fatness + m_fatness;
                currentNode->m_fatAABB.m_sizeY = collider->m_aabb.m_sizeY + m_fatness + m_fatness;
                insert(nodeIndex);
            }
        FE_END_PROFILE;
    }

void fe::aabbTree::colliderAABB(fe::AABB &testAABB, std::function<void(void*)> callback) const
    {
        void *colliderCallbacks[FE_MAX_GAME_OBJECTS];
        int colliderCallbackIndex = 0;
        FE_ENGINE_PROFILE("aabb_tree", "test_aabb_against_tree");
        int stack[(FE_MAX_GAME_OBJECTS * 2) - 1];
        int stackTop = 0;
        stack[stackTop++] = m_base;
        int iteration = 0;
        while (stackTop > 0)
            {
                iteration++;
                int currentNode = stack[stackTop - 1];
                stackTop--;
                if (currentNode >= 0 && m_nodes[currentNode].m_fatAABB.intersects(testAABB))
                    {
                        if (m_nodes[currentNode].isLeaf())
                            {
                                colliderCallbacks[colliderCallbackIndex++] = m_nodes[currentNode].m_userData;
                            }
                        else
                            {
                                stack[stackTop++] = m_nodes[currentNode].m_left;
                                stack[stackTop++] = m_nodes[currentNode].m_right;
                            }
                    }
                
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("aabb_tree", "collider_callback_calls");
        for (int i = 0; i < colliderCallbackIndex; i++)
            {
                callback(colliderCallbacks[i]);
            }
        FE_END_PROFILE;
    }

void *fe::aabbTree::colliderAtPoint(float x, float y) const
    {
        auto branch = pointCollideBranch(x, y, m_base);
        if (!branch) { return nullptr; }
        return static_cast<fe::aabbTree::treeNode*>(branch)->m_userData;
    }

fe::raycastResult fe::aabbTree::raycast(float x, float y, float dirX, float dirY) const
    {
        return fe::raycastResult();
    }

void fe::aabbTree::debugDraw()
    {
        if (m_debug)
            {
                debugDrawAABB(m_base);
            }
    }
