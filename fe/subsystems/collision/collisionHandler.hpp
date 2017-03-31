// collisionHandler.hpp
// a class that handles all collisions. It will detect all possible collisions, resolve them, and then call a function that dictates what happens
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/memoryManager.hpp"
#include <vector>

namespace fe
    {
        struct collider;
        class collisonHandler
            {
                private:
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
                auto buf = FE_ALLOC_STACK("CollisionHandler", sizeof(T));
                auto ret = new(buf) T(args...);
                m_colliders.push_back(ret);
                return ret;
            }
    }