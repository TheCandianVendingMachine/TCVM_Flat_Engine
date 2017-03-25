// collisionHandler.hpp
// a class that handles all collisions. It will detect all possible collisions, resolve them, and then call a function that dictates what happens
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/poolAllocater.hpp"
#include <vector>

namespace fe
    {
        struct collider;
        class collisonHandler
            {
                private:
                    fe::poolAllocater<collider> m_colliderPool;
                    std::vector<collider*> m_colliders;

                public:
                    FLAT_ENGINE_API collisonHandler();

                    template<typename T, typename ...Args>
                    FLAT_ENGINE_API T *add(Args ...args);
                    FLAT_ENGINE_API void remove(const collider *collider);

                    FLAT_ENGINE_API void handleCollisions();

            };

        template<typename T, typename ...Args>
        T *collisonHandler::add(Args ...args)
            {
                T *ref = m_colliderPool.alloc<T>(args...);
                m_colliders.emplace_back(ref);
                return ref;
            }
    }