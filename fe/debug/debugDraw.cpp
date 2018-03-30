#include "debugDraw.hpp"
#include "profiler.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::debugDraw *fe::debugDraw::m_instance = nullptr;

void fe::debugDraw::startUp()
    {
        if (!m_instance)
            {
                m_instance = this;
                m_drawVerticies.setPrimitiveType(sf::PrimitiveType::Lines);
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
        m_drawVerticies.append(sf::Vertex({ (float)x0, (float)y0 }, colour));
        m_drawVerticies.append(sf::Vertex({ (float)x1, (float)y1 }, colour));
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
        FE_LOG_ERROR("Circle Debug Not Supported");
        FE_END_PROFILE;
    }

void fe::debugDraw::draw(sf::RenderTarget &target)
    {
        FE_ENGINE_PROFILE("debug_draw", "draw_to_window");
        target.draw(m_drawVerticies);
        m_drawVerticies.clear();
        FE_END_PROFILE;
    }
