// collisionHandler.hpp
// a class that handles all collisions. It will detect all possible collisions, resolve them, and then call a function that dictates what happens
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <vector>

namespace fe
    {
        class collider;

        class collisonHandler
            {
                private:
                    std::vector<collider*> m_colliders;

                public:
                    void add(collider *collider);
                    void remove(collider *collider);

                    void handleCollisions();

            };
    }