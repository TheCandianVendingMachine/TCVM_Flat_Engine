// collisionHandler.hpp
// a class that handles all collisions. It will detect all possible collisions, resolve them, and then call a function that dictates what happens
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <vector>

namespace fe
    {
        struct collider;
        class collisonHandler
            {
                private:
                    std::vector<const collider*> m_colliders;

                public:
                    FLAT_ENGINE_API void add(const collider *collider);
                    FLAT_ENGINE_API void remove(const collider *collider);

                    FLAT_ENGINE_API void handleCollisions();

            };
    }