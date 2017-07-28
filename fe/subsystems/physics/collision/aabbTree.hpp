// aabbTree.hpp
// A broadphase algorithm that uses dynamic AABB's
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
                    struct node
                        {
                            fe::AABB m_fatAABB;
                            node *m_parent;
                            struct
                                {
                                    node *m_leftChild;
                                    node *m_rightChild;
                                };
                            fe::collider *m_userData;
                            bool isLeaf() const { return m_leftChild == nullptr; }
                            node *getSibling() const { return m_parent->m_leftChild != this ? m_parent->m_leftChild : m_parent->m_rightChild; }
                        };

                    fe::poolAllocater<node> m_nodes;
                    node *m_movedNodes[FE_MAX_GAME_OBJECTS];

                    node *m_base;
                    float m_fatness; // how much extra space the AABB contains
                    unsigned int m_movedNodesIndex;

                    FLAT_ENGINE_API void debugDrawAABB(node *base);

                    FLAT_ENGINE_API void updateAABB(node *baseNode);
                    FLAT_ENGINE_API void checkMovedColliders(node *baseNode);
                    FLAT_ENGINE_API void insert(node *baseNode, node *parent);

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
