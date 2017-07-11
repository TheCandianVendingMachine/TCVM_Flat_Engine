// debugDraw.hpp
// A singleton-class to allow for drawing for debug purposes. Software Drawn, so all shapes are wireframes
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include <SFML/Graphics/VertexArray.hpp>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class debugDraw
            {
                private:
                    sf::VertexArray m_drawVerticies;

                    static debugDraw *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API static debugDraw &get();

                    FLAT_ENGINE_API void addLine(int x0, int y0, int x1, int y1);
                    FLAT_ENGINE_API void addSquare(int xSize, int ySize, int xPos, int yPos);
                    FLAT_ENGINE_API void addCircle(int radius, int xPos, int yPos);

                    FLAT_ENGINE_API void draw(sf::RenderTarget &target);
            };
    }

#ifdef _DEBUG
    #define FE_DEBUG_DRAW_LINE(x0, y0, x1, y1) fe::debugDraw::get().addLine(x0, y0, x1, y1)
    #define FE_DEBUG_DRAW_SQUARE(xSize, ySize, xPos, yPos) fe::debugDraw::get().addSquare(xSize, ySize, xPos, yPos)
    #define FE_DEBUG_DRAW_CIRCLE(radius, xPos, yPos) fe::debugDraw::get().addCircle(radius, xPos, yPos)
#else
    #define FE_DEBUG_DRAW_LINE(x0, y0, x1, y1)
    #define FE_DEBUG_DRAW_SQUARE(xSize, ySize, xPos, yPos)
    #define FE_DEBUG_DRAW_CIRCLE(radius, xPos, yPos)
#endif