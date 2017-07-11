// debugDraw.hpp
// A singleton-class to allow for drawing for debug purposes. Software Drawn, so all shapes are wireframes
#pragma once
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
                    void startUp();
                    void shutDown();

                    static debugDraw &get();

                    void addLine(int x0, int y0, int x1, int y1);
                    void addSquare(int xSize, int ySize, int xPos, int yPos);
                    void addCircle(int radius, int xPos, int yPos);

                    void draw(sf::RenderTarget &target);
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