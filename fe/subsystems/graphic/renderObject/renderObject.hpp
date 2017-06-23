// renderObject.hpp
// an object that represents the coordinates for verticies to be rendered
#pragma once

namespace fe
    {
        struct renderObject
            {
                int m_vertColour[4];  // r, g, b, a
                float m_verticies[4]; // x, y, w, h
                float m_texCoords[2];// x, y
                bool m_draw = true;
            };
    }