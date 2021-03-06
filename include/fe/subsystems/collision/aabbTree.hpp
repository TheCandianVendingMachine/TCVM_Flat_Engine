// aabbTree.hpp
// A broadphase algorithm that uses dynamic AABB's
// Implementation taken from Box2d (https://github.com/erincatto/Box2D/blob/master/Box2D/Box2D/Collision/b2DynamicTree.cpp)
#pragma once
#include "fe/flatEngineExport.hpp"
#include "broadphaseAbstract.hpp"
#include "bounds/AABB.hpp"
#include "fe/subsystems/memory/poolAllocater.hpp"
#include <stack>

namespace fe
    {
        struct collider;
        class aabbTree : public broadphaseAbstract
            {
                private:
                    struct treeNode
                        {
                            static const int Null = -1;    
                            fe::AABB m_fatAABB;
                            union
                                {
                                    int m_parent;
                                    int m_next;
                                };

                            struct
                                {
                                    int m_left = -1;
                                    int m_right = -1;
                                };

                            fe::collider *m_userData;

                            int m_height;

                            bool isLeaf() const { return m_right == treeNode::Null; }
                        } *m_nodes;

                    unsigned int m_nodeCapacity;
                    unsigned int m_nodeCount;
                    unsigned int m_freeList;

                    int m_base;
                    float m_fatness; // how much extra space the AABB contains

                    FLAT_ENGINE_API void debugDrawAABB(int node);

                    FLAT_ENGINE_API void freeNode(int node);
                    FLAT_ENGINE_API int allocateNode();
                    FLAT_ENGINE_API void insert(int node);
                    FLAT_ENGINE_API void remove(int node);
                    FLAT_ENGINE_API int balance(int node);

                    FLAT_ENGINE_API void *pointCollideBranch(float x, float y, int branch) const;

                    friend class collisionWorld;

                public:
                    FLAT_ENGINE_API aabbTree();
                    FLAT_ENGINE_API ~aabbTree();

                    FLAT_ENGINE_API void startUp();

                    // Adds an AABB to the broadphase algorithm
                    FLAT_ENGINE_API void add(fe::collider *collider);

                    // Removes  an AABB from the tree
                    FLAT_ENGINE_API void remove(fe::collider *collider);

                    // Updates all colliders in the broadphase algorithm
                    FLAT_ENGINE_API void update(fe::collider *collider);

                    FLAT_ENGINE_API void colliderAABB(fe::AABB &testAABB, std::function<void(fe::collider*)> callback) const;
                    FLAT_ENGINE_API void colliderCircle(fe::circle &testCircle, std::function<void(fe::collider*)> callback) const;

                    // Returns the collider that is at the point
                    FLAT_ENGINE_API void *colliderAtPoint(float x, float y) const;

                    // Casts a ray and tests against the broadphase algorithm
                    FLAT_ENGINE_API fe::raycastResult raycast(float x, float y, float dirX, float dirY, fe::str ignoreGroup) const;

                    FLAT_ENGINE_API fe::raycastResult linecast(float x0, float y0, float x1, float y1, fe::str ignoreGroup) const;

                    FLAT_ENGINE_API void debugDraw();

            };
    }