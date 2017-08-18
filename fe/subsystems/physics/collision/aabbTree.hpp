// aabbTree.hpp
// A broadphase algorithm that uses dynamic AABB's
// Implementation taken from Box2d (https://github.com/erincatto/Box2D/blob/master/Box2D/Box2D/Collision/b2DynamicTree.cpp)
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "broadphaseAbstract.hpp"
#include "collisionBounds.hpp"
#include "../../memory/poolAllocater.hpp"

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

                            union
                                {
                                    struct
                                        {
                                            int m_left;
                                            int m_right;
                                        };

                                    void *m_userData;
                                };

                            int m_height;

                            bool isLeaf() { return m_right == treeNode::Null; }
                        } m_nodes[(FE_MAX_GAME_OBJECTS * 2) - 1];

                    const unsigned int m_nodeCapacity = (FE_MAX_GAME_OBJECTS * 2) - 1;
                    unsigned int m_freeList;

                    unsigned int m_base;
                    float m_fatness; // how much extra space the AABB contains

                    FLAT_ENGINE_API void debugDrawAABB(int node);

                    FLAT_ENGINE_API void freeNode(int node);
                    FLAT_ENGINE_API int allocateNode();
                    FLAT_ENGINE_API void insert(int node);
                    FLAT_ENGINE_API void remove(int node);
                    FLAT_ENGINE_API int balance(int node);

                public:
                    FLAT_ENGINE_API aabbTree();

                    FLAT_ENGINE_API void startUp();

                    // Adds an AABB to the broadphase algorithm
                    FLAT_ENGINE_API void add(fe::collider *collider);

                    // Removes  an AABB from the tree
                    FLAT_ENGINE_API void remove(fe::collider *collider);

                    // Updates all colliders in the broadphase algorithm
                    FLAT_ENGINE_API void update();

                    // Returns a list of the colliders that are possibly intersecting
                    FLAT_ENGINE_API const std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int> computeColliderPairs();

                    // Returns the collider that is at the point
                    FLAT_ENGINE_API fe::collider *colliderAtPoint(float x, float y);

                    // Returns an array of objects that collide with the AABB argument.
                    // First return value is how many objects collide
                    // Second return value is the array of objects that have collided
                    FLAT_ENGINE_API std::pair<fe::collider*[FE_MAX_GAME_OBJECTS], unsigned int> collidesWithAABB(fe::collider &aabb);

                    // Casts a ray and tests against the broadphase algorithm
                    FLAT_ENGINE_API fe::raycastResult raycast(float x, float y, float dirX, float dirY);

            };
    }