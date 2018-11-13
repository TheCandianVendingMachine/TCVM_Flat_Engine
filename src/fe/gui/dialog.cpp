#include "fe/gui/dialog.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/engine.hpp"
#include <algorithm>

fe::gui::dialog::dialog() :
    m_dialogState(dialogStates::NONE),
    m_killed(false),
    m_draw(true)
    {
    }

void fe::gui::dialog::kill()
    {
        m_killed = true;
    }

bool fe::gui::dialog::isKilled() const
    {
        return m_killed;
    }

void fe::gui::dialog::setDraw(bool draw)
    {
        m_draw = draw;
    }

bool fe::gui::dialog::isDrawn() const
    {
        return m_draw;
    }

void fe::gui::dialog::setState(dialogStates state)
    {
        onStateChange(m_dialogState, state);
        m_dialogState = state;
    }

fe::gui::dialogStates fe::gui::dialog::getState() const
    {
        return m_dialogState;
    }

void fe::gui::dialog::draw(sf::RenderTarget &target, const fe::transformable &matrix)
    {
        fe::transformable transform = matrix;
        transform.combine(m_drawMatrix);
        FE_ENGINE_PROFILE("gui_dialog", "draw_elements");
        drawDialogElements(target, transform);
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("gui_dialog", "draw_text");
        drawDialogText(target, transform);
        FE_END_PROFILE;
    }

fe::transformable &fe::gui::dialog::getDrawMatrix()
    {
        return m_drawMatrix;
    }

fe::gui::dialog &fe::gui::dialog::operator=(dialog &rhs)
    {
        if (&rhs != this)
            {
                m_killed = rhs.m_killed;
            }
        return *this;
    }

void fe::gui::dialog::setPosition(float x, float y, fe::gui::align alignment)
    {
        fe::lightVector2d alignPos(0.f, 0.f);
        fe::lightVector2d size = getSize();
        switch (alignment)
            {
                case fe::gui::align::MIDDLE:
                    alignPos = size / 2.f;
                    break;
                case fe::gui::align::TOP_LEFT:
                    alignPos.x = 0.f;
                    alignPos.y = 0.f;
                    break;
                case fe::gui::align::TOP_MIDDLE:
                    alignPos.x = size.x / 2.f;
                    alignPos.y = 0.f;
                    break;
                case fe::gui::align::TOP_RIGHT:
                    alignPos.x = size.x;
                    alignPos.y = 0.f;
                    break;
                case fe::gui::align::RIGHT:
                    alignPos.x = size.x;
                    alignPos.y = size.y / 2.f;
                    break;
                case fe::gui::align::BOTTOM_RIGHT:
                    alignPos.x = size.x;
                    alignPos.y = size.y;
                    break;
                case fe::gui::align::BOTTOM_MIDDLE:
                    alignPos.x = size.x / 2.f;
                    alignPos.y = size.y;
                    break;
                case fe::gui::align::BOTTOM_LEFT:
                    alignPos.x = 0.f;
                    alignPos.y = size.y;
                    break;
                case fe::gui::align::LEFT:
                    alignPos.x = 0.f;
                    alignPos.y = size.y / 2.f;
                    break;
                default:
                    break;
            }

        fe::transformable::setPosition(x - alignPos.x, y - alignPos.y);
    }

void fe::gui::dialog::setPosition(fe::Vector2d pos, fe::gui::align alignment)
    {
        setPosition(pos.x, pos.y, alignment);
    }

void fe::gui::dialog::setWindowPosition(float x, float y, fe::gui::align alignment)
    {
        fe::Vector2<unsigned int> windowSize = fe::engine::get().getWindowSize();

        fe::lightVector2d alignPos(0.f, 0.f);
        fe::lightVector2d size = getSize();
        switch (alignment)
            {
                case fe::gui::align::MIDDLE:
                    alignPos = size / 2.f;
                    break;
                case fe::gui::align::TOP_LEFT:
                    alignPos.x = 0.f;
                    alignPos.y = 0.f;
                    break;
                case fe::gui::align::TOP_MIDDLE:
                    alignPos.x = size.x / 2.f;
                    alignPos.y = 0.f;
                    break;
                case fe::gui::align::TOP_RIGHT:
                    alignPos.x = size.x;
                    alignPos.y = 0.f;
                    break;
                case fe::gui::align::RIGHT:
                    alignPos.x = size.x;
                    alignPos.y = size.y / 2.f;
                    break;
                case fe::gui::align::BOTTOM_RIGHT:
                    alignPos.x = size.x;
                    alignPos.y = size.y;
                    break;
                case fe::gui::align::BOTTOM_MIDDLE:
                    alignPos.x = size.x / 2.f;
                    alignPos.y = size.y;
                    break;
                case fe::gui::align::BOTTOM_LEFT:
                    alignPos.x = 0.f;
                    alignPos.y = size.y;
                    break;
                case fe::gui::align::LEFT:
                    alignPos.x = 0.f;
                    alignPos.y = size.y / 2.f;
                    break;
                default:
                    break;
            }

        fe::transformable::setPosition((windowSize.x * x) - alignPos.x, (windowSize.y * y) - alignPos.y);
    }

void fe::gui::dialog::setWindowPosition(fe::Vector2d pos, fe::gui::align alignment)
    {
        setWindowPosition(pos.x, pos.y, alignment);
    }

fe::lightVector2d fe::gui::dialog::getSize() const
    {
        return fe::lightVector2d(0.f, 0.f);
    }

void fe::gui::dialog::setGraphNode(int node)
    {
        m_graphNode = node;
    }

int fe::gui::dialog::getGraphNode()
    {
        return m_graphNode;
    }
