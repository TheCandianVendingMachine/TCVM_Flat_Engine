// renderObject.hpp
// an object that represents the coordinates for verticies to be rendered
#pragma once
#include "../../physics/transformable.hpp"

namespace fe
    {
        struct renderObject
            {
                fe::transformable m_transform;
                int m_vertColour[4];  // r, g, b, a
                float m_verticies[4]; // x, y, w, h
                float m_texCoords[2];// x, y
                bool m_draw = true;
                bool m_static = false;
            };
    }