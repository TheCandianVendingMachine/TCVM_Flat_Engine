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
        class aabbTree : public broadphaseAbstract
            {
                private:
                    struct node 
                        {
                            node *parent;
                            node *children[2];

                            bool childrenCrossed;
                            fe::AABB aabb;
                            fe::AABB *data;

                            node();
                            bool isLeaf();
                            void setBranch(node *n0, node *n1);
                            void setLeaf(fe::AABB *data);
                            void updateAABB(float margin);
                        };

                    void updateNodeHelper(node *base, std::vector<node*> &invalidNodes);
                    void insertNode(node *base, node **parent);
                    void removeNode(node *base);
                    void computePairsHelper(node *n0, node *n1);
                    void clearChildrenCrossFlagHelper(node *base);
                    void crossChildren(node *base);

                    node *m_root;
                    std::list<std::pair<collider*, collider*>> m_pairs;
                    float m_margin;
                    std::vector<node*> m_invalidNodes;

                public:
                    aabbTree();

                    // Adds an AABB to the broadphase algorithm
                    void add(fe::collider *collider);

                    // Removes  an AABB from the tree
                    void remove(fe::collider *collider);

                    // Updates all colliders in the broadphase algorithm
                    void update();

                    // Returns a list of the colliders that are possibly intersecting
                    const std::list<std::pair<fe::collider*, fe::collider*>> computeColliderPairs();

                    // Returns the collider that is at the point
                    fe::collider *colliderAtPoint(float x, float y);

                    // Returns an array of objects that collide with the AABB argument.
                    // First return value is how many objects collide
                    // Second return value is the array of objects that have collided
                    std::pair<unsigned int, fe::collider*[FE_MAX_GAME_OBJECTS]> collidesWithAABB(fe::collider &aabb);

                    // Casts a ray and tests against the broadphase algorithm
                    fe::raycastResult raycast(float x, float y, float dirX, float dirY);

            };
    }
