// mixedTests.hpp
// Collision Bound vs Collision Bound tests for all bounds
#pragma once
#include "fe/flatEngineExport.hpp"

namespace fe
    {
        struct AABB;
        struct circle;

        // Returns if B intersects A
        FLAT_ENGINE_API bool intersects(const fe::AABB &a, const fe::circle &b);
        // Returns if B intersects A
        FLAT_ENGINE_API bool intersects(const fe::circle &a, const fe::AABB &b);


    }
