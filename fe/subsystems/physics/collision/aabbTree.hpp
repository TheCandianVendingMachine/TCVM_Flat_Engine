// aabbTree.hpp
// A broadphase algorithm that uses dynamic AABB's
#pragma once
#define FLAT_ENGINE_EXPORT
#include <vector>
#include "../../../flatEngineExport.hpp"
#include "broadphaseAbstract.hpp"
#include "collisionBounds.hpp"

namespace fe
    {
        struct collider;
        // Created using http://allenchou.net/2014/02/game-physics-broadphase-dynamic-aabb-tree/
        class aabbTree : public broadphaseAbstract
            {
                private:
                    struct node 
                        {
                            node *parent;
                            node *children[2];

                            bool childrenCrossed;
                            fe::AABB aabb;
                            fe::collider *data;

                            FLAT_ENGINE_API node();
                            FLAT_ENGINE_API bool isLeaf() const;
                            FLAT_ENGINE_API void setBranch(node *n0, node *n1);
                            FLAT_ENGINE_API void setLeaf(fe::collider *data);
                            FLAT_ENGINE_API void updateAABB(float margin);
                            FLAT_ENGINE_API node *getSibling() const;
                        };

                    FLAT_ENGINE_API void updateNodeHelper(node *base, node *invalidNodes[FE_MAX_GAME_OBJECTS]);
                    FLAT_ENGINE_API void insertNode(node *base, node **parent);
                    FLAT_ENGINE_API void removeNode(node *base);
                    FLAT_ENGINE_API void computePairsHelper(node *n0, node *n1);
                    FLAT_ENGINE_API void clearChildrenCrossFlagHelper(node *base);
                    FLAT_ENGINE_API void crossChildren(node *base);

                    FLAT_ENGINE_API void drawAABB(node *base);

                    std::pair<collider*, collider*> m_pairs[FE_MAX_GAME_OBJECTS];
                    node *m_invalidNodes[FE_MAX_GAME_OBJECTS];

                    unsigned int m_pairsIndex;
                    unsigned int m_invalidNodesIndex;

                    node *m_root;
                    float m_margin;

                public:
                    FLAT_ENGINE_API aabbTree();

                    // Adds an AABB to the broadphase algorithm
                    FLAT_ENGINE_API void add(fe::collider *collider);

                    // Removes  an AABB from the tree
                    FLAT_ENGINE_API void remove(fe::collider *collider);

                    // Updates all colliders in the broadphase algorithm
                    FLAT_ENGINE_API void update(float dt);

                    // Returns a list of the colliders that are possibly intersecting
                    FLAT_ENGINE_API const std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int> computeColliderPairs();

                    // Returns the collider that is at the point
                    FLAT_ENGINE_API fe::collider *colliderAtPoint(float x, float y);

                    // Returns an array of objects that collide with the AABB argument.
                    // First return value is how many objects collide
                    // Second return value is the array of objects that have collided
                    FLAT_ENGINE_API std::pair<unsigned int, fe::collider*[FE_MAX_GAME_OBJECTS]> collidesWithAABB(fe::collider &aabb);

                    // Casts a ray and tests against the broadphase algorithm
                    FLAT_ENGINE_API fe::raycastResult raycast(float x, float y, float dirX, float dirY);

            };
    }
