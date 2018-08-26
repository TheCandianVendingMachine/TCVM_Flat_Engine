// polygon2.hpp
// A general 2d polygon. Points are defined and can be iterated through
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/math/Vector2.hpp"
#include <vector>
#include <array>

namespace fe
    {
        /*
            POLYGONS MUST BE CONSTRUCTED VIA Counter Clock Wise COORDINATES
            Creation of polygon may fail if this is not met
        */
        struct polygon2d
            {
                // Vector of triangles created by triangulated polygon. Triangles are clockwise
                std::vector<std::array<fe::lightVector2d, 3>> m_verticies;
                std::vector<fe::lightVector2d> m_points;

                FLAT_ENGINE_API polygon2d() {}
                FLAT_ENGINE_API polygon2d(const std::initializer_list<fe::lightVector2d> &points);

                FLAT_ENGINE_API void addPoint(fe::lightVector2d point);
                FLAT_ENGINE_API void addPoint(float x, float y);
                FLAT_ENGINE_API void createPolygon(const std::initializer_list<fe::lightVector2d> &points);
                FLAT_ENGINE_API void createPolygon();

                FLAT_ENGINE_API bool pointInPolygon(fe::lightVector2d point);
                FLAT_ENGINE_API bool pointInPolygon(float x, float y);

                FLAT_ENGINE_API void clear();

            };
    }
