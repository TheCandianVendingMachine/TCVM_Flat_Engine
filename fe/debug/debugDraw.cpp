#include "debugDraw.hpp"
#include "profiler.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::debugDraw *fe::debugDraw::m_instance = nullptr;

void fe::debugDraw::startUp()
    {
        if (!m_instance)
            {
                m_instance = this;
                m_drawVerticies.setPrimitiveType(sf::PrimitiveType::Points);
            }
    }

void fe::debugDraw::shutDown()
    {
        m_instance = nullptr;
    }

fe::debugDraw &fe::debugDraw::get()
    {
        return *m_instance;
    }

void fe::debugDraw::addLine(int x0, int y0, int x1, int y1, sf::Color colour)
    {
        FE_ENGINE_PROFILE("debug_draw", "add_line");
        bool steep = false;
        if (abs(x0 - x1) < abs(y0 - y1))
            {
                std::swap(x0, y0);
                std::swap(x1, y1);
                steep = true;
            }

        if (x0 > x1)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }

        int dx = x1 - x0;
        int dy = y1 - y0;

        int dError = abs(dy) * 2;
        int error = 0;
        int y = y0;

        for (int x = x0; x <= x1; x++)
            {
                if (steep)
                    {
                        m_drawVerticies.append(sf::Vertex({ (float)y, (float)x }, colour));
                    }
                else
                    {
                        m_drawVerticies.append(sf::Vertex({ (float)x, (float)y }, colour));
                    }
                error += dError;
                if (error > 0.5f)
                    {
                        y += (y1 > y0 ? 1 : -1);
                        error -= dx * 2;
                    }
            }
        FE_END_PROFILE;
    }

void fe::debugDraw::addSquare(int xSize, int ySize, int xPos, int yPos, sf::Color colour)
    {
        FE_ENGINE_PROFILE("debug_draw", "add_square");
        addLine(xPos, yPos,                     xPos + xSize,   yPos, colour);
        addLine(xPos + xSize,   yPos,           xPos + xSize,   yPos + ySize, colour);
        addLine(xPos + xSize,   yPos + ySize,   xPos,           yPos + ySize, colour);
        addLine(xPos,           yPos + ySize,   xPos,           yPos, colour);
        FE_END_PROFILE;
    }

void fe::debugDraw::addCircle(int radius, int xPos, int yPos, sf::Color colour)
    {
        FE_ENGINE_PROFILE("debug_draw", "add_circle");
        int x = radius - 1;
        int y = 0;
        int dx = 1;
        int dy = 1;
        int err = dx - (radius << 1);

        while (x >= y)
            {
                sf::Vertex appVert({ (float)xPos + x, (float)yPos + y }, colour); m_drawVerticies.append(appVert);
                appVert.position.x = xPos + y; appVert.position.y = yPos + x; m_drawVerticies.append(appVert);
                appVert.position.x = xPos - y; appVert.position.y = yPos + x; m_drawVerticies.append(appVert);
                appVert.position.x = xPos - x; appVert.position.y = yPos + y; m_drawVerticies.append(appVert);
                appVert.position.x = xPos - x; appVert.position.y = yPos - y; m_drawVerticies.append(appVert);
                appVert.position.x = xPos - y; appVert.position.y = yPos - x; m_drawVerticies.append(appVert);
                appVert.position.x = xPos + y; appVert.position.y = yPos - x; m_drawVerticies.append(appVert);
                appVert.position.x = xPos + x; appVert.position.y = yPos - y; m_drawVerticies.append(appVert);

                if (err <= 0)
                    {
                        y++;
                        err += dy;
                        dy += 2;
                    }
                else
                    {
                        x--;
                        dx += 2;
                        err += (-radius << 1) + dx;
                    }
            }
        FE_END_PROFILE;
    }

void fe::debugDraw::draw(sf::RenderTarget &target)
    {
        FE_ENGINE_PROFILE("debug_draw", "draw_to_window");
        target.draw(m_drawVerticies);
        m_drawVerticies.clear();
        FE_END_PROFILE;
    }
