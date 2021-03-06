#include "fe/subsystems/collision/aabbTree.hpp"
#include "fe/subsystems/collision/collisionBody.hpp"
#include "fe/subsystems/collision/bounds/aabbTests.hpp"
#include "fe/subsystems/collision/bounds/circle.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/subsystems/collision/bounds/mixedTests.hpp"

#include "fe/debug/debugDraw.hpp"
#include "fe/debug/profiler.hpp"

#include "fe/dataStructures/stack.hpp"

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

        fe::AABB &currentNodeFatAABB = m_nodes[node].m_fatAABB;

        int xPos = static_cast<int>(std::ceilf(currentNodeFatAABB.m_globalPositionX + currentNodeFatAABB.m_offsetX));
        int yPos = static_cast<int>(std::ceilf(currentNodeFatAABB.m_globalPositionY + currentNodeFatAABB.m_offsetY));
        int xSize = static_cast<int>(std::ceilf(currentNodeFatAABB.m_sizeX));
        int ySize = static_cast<int>(std::ceilf(currentNodeFatAABB.m_sizeY));

        FE_DEBUG_DRAW_SQUARE(xSize, ySize, xPos, yPos, sf::Color::Red);

        if (m_nodes[node].m_userData)
            {
                fe::AABB &currentNodeAABB = m_nodes[node].m_userData->m_aabb;

                xPos = static_cast<int>(std::ceilf(currentNodeAABB.m_globalPositionX + currentNodeAABB.m_offsetX));
                yPos = static_cast<int>(std::ceilf(currentNodeAABB.m_globalPositionY + currentNodeAABB.m_offsetY));
                xSize = static_cast<int>(std::ceilf(currentNodeAABB.m_sizeX));
                ySize = static_cast<int>(std::ceilf(currentNodeAABB.m_sizeY));

                FE_DEBUG_DRAW_SQUARE(xSize, ySize, xPos, yPos, sf::Color(255, 221, 221));
            }
    }

void fe::aabbTree::freeNode(int node)
    {
        FE_ENGINE_PROFILE("aabb_tree", "free_node");
        m_nodes[node].m_next = m_freeList;
        m_nodes[node].m_height = -1;
        m_freeList = node;
        --m_nodeCount;
        FE_END_PROFILE;
    }

int fe::aabbTree::allocateNode()
    {
        FE_ENGINE_PROFILE("aabb_tree", "allocate_node");
        
        if (m_freeList == treeNode::Null)
            {
                FE_ASSERT(m_nodeCount == m_nodeCapacity, "Allocation Failure");

                treeNode *oldNodes = m_nodes;

                m_nodeCapacity *= 2;
                m_nodes = (treeNode*)calloc(m_nodeCapacity, sizeof(treeNode));
                std::memcpy(m_nodes, oldNodes, m_nodeCount * sizeof(treeNode));
                free(oldNodes);

                for (unsigned int i = m_nodeCount; i < m_nodeCapacity - 1; i++)
                    {
                        m_nodes[i].m_next = i + 1;
                        m_nodes[i].m_height = -1;
                    }

                m_nodes[m_nodeCapacity - 1].m_next = treeNode::Null;
                m_nodes[m_nodeCapacity - 1].m_height = -1;
                m_freeList = m_nodeCount;
            }

        int nodeId = m_freeList;
        m_freeList = m_nodes[nodeId].m_next;
        m_nodes[nodeId].m_parent = treeNode::Null;
        m_nodes[nodeId].m_left = treeNode::Null;
        m_nodes[nodeId].m_right = treeNode::Null;
        m_nodes[nodeId].m_height = 0;
        m_nodes[nodeId].m_userData = nullptr;
        ++m_nodeCount;
        return nodeId;
        FE_END_PROFILE;
    }

void fe::aabbTree::insert(int node)
    {
        FE_ENGINE_PROFILE("aabb_tree", "insert_index");
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
                        cost2 = (newArea - oldArea) + inheritCost;
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
        FE_ENGINE_PROFILE("aabb_tree", "balance_tree");
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
        if (fe::contains(m_nodes[branch].m_fatAABB, x, y))
            {
                if (m_nodes[branch].isLeaf())
                    {
                        if (fe::contains(m_nodes[branch].m_userData->m_aabb, x, y))
                            {
                                return (void*)(&m_nodes[branch]);
                            }
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

fe::aabbTree::aabbTree() : 
    m_nodes(nullptr),
    m_base(treeNode::Null),
    m_fatness(5.f),
    m_nodeCapacity((FE_MAX_GAME_OBJECTS * 2) - 1),
    m_nodeCount(0)
    {
        int i = 0;
    }

fe::aabbTree::~aabbTree()
    {
        free(m_nodes);
    }

void fe::aabbTree::startUp()
    {
        m_nodes = (treeNode*)malloc(m_nodeCapacity * sizeof(treeNode));
        std::memset(m_nodes, 0, m_nodeCapacity * sizeof(treeNode));

        for (unsigned int i = 0; i < m_nodeCapacity - 1; i++)
            {
                m_nodes[i].m_next = i + 1;
                m_nodes[i].m_height = -1;
            }
        m_nodes[m_nodeCapacity - 1].m_next = treeNode::Null;
        m_nodes[m_nodeCapacity - 1].m_height = -1;

        m_freeList = 0;
    }

void fe::aabbTree::add(fe::collider *collider)
    {
        FE_ENGINE_PROFILE("aabb_tree", "add_collider");
        int colliderId = allocateNode();
        m_nodes[colliderId].m_userData = collider;
        collider->m_userData = (void*)(colliderId);
        m_nodes[colliderId].m_height = 0;
        
        treeNode *currentNode = &m_nodes[colliderId];
        currentNode->m_fatAABB.m_globalPositionX = collider->m_aabb.m_globalPositionX - m_fatness + collider->m_aabb.m_offsetX;
        currentNode->m_fatAABB.m_globalPositionY = collider->m_aabb.m_globalPositionY - m_fatness + collider->m_aabb.m_offsetY;
        currentNode->m_fatAABB.m_sizeX = collider->m_aabb.m_sizeX + m_fatness + m_fatness;
        currentNode->m_fatAABB.m_sizeY = collider->m_aabb.m_sizeY + m_fatness + m_fatness;

        insert(colliderId);
        FE_END_PROFILE;
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
        if (!collider) return;
        FE_ENGINE_PROFILE("aabb_tree", "remove_collider");
        remove(int(collider->m_userData));
        collider->m_userData = nullptr;
        FE_END_PROFILE;
    }

void fe::aabbTree::update(fe::collider *collider)
    {
        if (!collider->m_enabled) return;

        FE_ENGINE_PROFILE("aabb_tree", "update_collider");
        int nodeIndex = int(collider->m_userData);

        if (!fe::contains(m_nodes[nodeIndex].m_fatAABB, collider->m_aabb))
            {
                remove(nodeIndex);
                treeNode *currentNode = &m_nodes[nodeIndex];
                currentNode->m_fatAABB.m_globalPositionX = collider->m_aabb.m_globalPositionX - m_fatness + collider->m_aabb.m_offsetX;
                currentNode->m_fatAABB.m_globalPositionY = collider->m_aabb.m_globalPositionY - m_fatness + collider->m_aabb.m_offsetY;
                currentNode->m_fatAABB.m_sizeX = collider->m_aabb.m_sizeX + m_fatness + m_fatness;
                currentNode->m_fatAABB.m_sizeY = collider->m_aabb.m_sizeY + m_fatness + m_fatness;
                insert(nodeIndex);
            }
        FE_END_PROFILE;
    }

void fe::aabbTree::colliderAABB(fe::AABB &testAABB, std::function<void(fe::collider*)> callback) const
    {
        fe::semiFixedStack<FE_MAX_GAME_OBJECTS, fe::collider*> colliderCallbacks;
        FE_ENGINE_PROFILE("aabb_tree", "test_aabb_against_tree");
        fe::semiFixedStack<(FE_MAX_GAME_OBJECTS * 2) - 1, int> stack;
        stack.push(m_base);
        while (!stack.empty())
            {
                int currentNode = stack.top();
                stack.pop();
                if (currentNode >= 0 && &m_nodes[currentNode].m_userData->m_aabb != &testAABB && fe::intersects(m_nodes[currentNode].m_fatAABB, testAABB))
                    {
                        if (m_nodes[currentNode].isLeaf())
                            {
                                if (/*m_nodes[currentNode].m_userData->m_collisionGroup != 0 &&*/ m_nodes[currentNode].m_userData->m_aabb.perimeter() > 0)
                                    {
                                        colliderCallbacks.push(m_nodes[currentNode].m_userData);
                                    }
                            }
                        else
                            {
                                stack.push(m_nodes[currentNode].m_left);
                                stack.push(m_nodes[currentNode].m_right);
                            }
                    }
                
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("aabb_tree", "collider_callback_calls_aabb");
        while (!colliderCallbacks.empty())
            {
                callback(colliderCallbacks.top());
                colliderCallbacks.pop();
            }
        FE_END_PROFILE;
    }

void fe::aabbTree::colliderCircle(fe::circle &testCircle, std::function<void(fe::collider*)> callback) const
    {
        fe::semiFixedStack<FE_MAX_GAME_OBJECTS, fe::collider*> colliderCallbacks;
        FE_ENGINE_PROFILE("aabb_tree", "test_aabb_against_tree");
        fe::semiFixedStack<(FE_MAX_GAME_OBJECTS * 2) - 1, int> stack;
        stack.push(m_base);
        while (!stack.empty())
            {
                int currentNode = stack.top();
                stack.pop();
                if (currentNode >= 0 && fe::intersects(m_nodes[currentNode].m_fatAABB, testCircle))
                    {
                        if (m_nodes[currentNode].isLeaf())
                            {
                                if (/*m_nodes[currentNode].m_userData->m_collisionGroup != 0 &&*/ m_nodes[currentNode].m_userData->m_aabb.perimeter() > 0)
                                    {
                                        colliderCallbacks.push(m_nodes[currentNode].m_userData);
                                    }
                            }
                        else
                            {
                                stack.push(m_nodes[currentNode].m_left);
                                stack.push(m_nodes[currentNode].m_right);
                            }
                    }
                
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("aabb_tree", "collider_callback_calls_aabb");
        while (!colliderCallbacks.empty())
            {
                callback(colliderCallbacks.top());
                colliderCallbacks.pop();
            }
        FE_END_PROFILE;
    }

void *fe::aabbTree::colliderAtPoint(float x, float y) const
    {
        auto branch = pointCollideBranch(x, y, m_base);
        if (!branch) { return nullptr; }
        return static_cast<fe::aabbTree::treeNode*>(branch)->m_userData;
    }

fe::raycastResult fe::aabbTree::raycast(float x, float y, float dirX, float dirY, fe::str ignoreGroup) const
    {
        FE_ENGINE_PROFILE("aabb_tree", "test_ray_against_tree");

        if (m_debug)
            {
                FE_DEBUG_DRAW_LINE(x, y, x + (dirX * 100.f), y + (dirY * 100.f), sf::Color::Red);
            }
        
        fe::semiFixedStack<(FE_MAX_GAME_OBJECTS * 2) - 1, int> stack;
        stack.push(m_base);
        while (!stack.empty())
            {
                int currentNode = stack.top();
                stack.pop();
                fe::raycastResult result = fe::rayIntersects(m_nodes[currentNode].m_fatAABB, x, y, dirX, dirY);
                if (currentNode >= 0 && result.m_hit)
                    {
                        if (m_nodes[currentNode].isLeaf())
                            {
                                if (m_nodes[currentNode].m_userData->m_collisionGroup != ignoreGroup && m_nodes[currentNode].m_userData->m_aabb.perimeter() > 0)
                                    {
                                        fe::raycastResult ray = fe::lineIntersects(m_nodes[currentNode].m_userData->m_aabb, x, y, dirX, dirY);
                                        if (ray.m_hit) 
                                            {
                                                ray.m_colliderHit = m_nodes[currentNode].m_userData;
                                                return ray;
                                            }
                                    }
                            }
                        else
                            {
                                stack.push(m_nodes[currentNode].m_left);
                                stack.push(m_nodes[currentNode].m_right);
                            }
                    }

            }
        FE_END_PROFILE;

        return fe::raycastResult();
    }

fe::raycastResult fe::aabbTree::linecast(float x0, float y0, float x1, float y1, fe::str ignoreGroup) const
    {
        FE_ENGINE_PROFILE("aabb_tree", "test_line_against_tree");

        if (m_debug)
            {
                FE_DEBUG_DRAW_LINE(x0, y0, x1, y1, sf::Color::Red);
            }

        fe::nonFixedStack<int> stack;
        stack.push(m_base);
        while (!stack.empty())
            {
                int currentNode = stack.top();
                stack.pop();
                fe::raycastResult result = fe::lineIntersects(m_nodes[currentNode].m_fatAABB, x0, y0, x1, y1);
                if (currentNode >= 0 && result.m_hit)
                    {
                        if (m_nodes[currentNode].isLeaf())
                            {
                                if (m_nodes[currentNode].m_userData->m_collisionGroup != ignoreGroup && m_nodes[currentNode].m_userData->m_aabb.perimeter() > 0)
                                    {
                                        fe::raycastResult ray = fe::lineIntersects(m_nodes[currentNode].m_userData->m_aabb, x0, y0, x1, y1);
                                        if (ray.m_hit) 
                                            {
                                                ray.m_colliderHit = m_nodes[currentNode].m_userData;
                                                return ray;
                                            }
                                    }
                            }
                        else
                            {
                                stack.push(m_nodes[currentNode].m_left);
                                stack.push(m_nodes[currentNode].m_right);
                            }
                    }

            }
        FE_END_PROFILE;

        return fe::raycastResult();
    }

void fe::aabbTree::debugDraw()
    {
        if (m_debug)
            {
                debugDrawAABB(m_base);
            }
    }
