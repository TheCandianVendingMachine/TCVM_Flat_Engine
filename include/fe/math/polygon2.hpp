// polygon2.hpp
// A general 2d polygon. Points are defined and can be iterated through
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/math/Vector2.hpp"
#include <vector>
#include <array>

namespace fe
    {
        struct polygon2d
            {
                // Vector of triangles created by triangulated polygon. Triangles are clockwise
                std::vector<std::array<fe::lightVector2d, 3>> m_verticies;

                FLAT_ENGINE_API polygon2d() {}
                FLAT_ENGINE_API polygon2d(const std::initializer_list<fe::lightVector2d> &points);

                FLAT_ENGINE_API void createPolygon(const std::initializer_list<fe::lightVector2d> &points);

                FLAT_ENGINE_API bool pointInPolygon(float x, float y);

            };
    }
