#include "fe/gui/checkList.hpp"
#include "fe/gui/guiGraph.hpp"

void fe::gui::checkList::onStateChange(dialogStates previous, dialogStates next)
    {
        
    }

void fe::gui::checkList::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        drawPolygon(getControlPolygon(), target, drawMatrix, getDrawColour());
    }

fe::gui::checkList::checkList(float checkBoxSize, unsigned int count, mode boxMode, float gapBetweenBoxes, const sf::Font *font, std::initializer_list<std::string> options) :
    m_boxSize(checkBoxSize),
    m_gapBetweenBoxes(gapBetweenBoxes),
    m_optionCount((int)options.size() - 1)
    {
        addPoint(0.f, 0.f);
        addPoint(20.f, 0.f);
        addPoint(20.f, 20.f);
        addPoint(0.f, 20.f);
        getControlPolygon().createPolygon();

        fe::lightVector2d boxPositionIncrement(0.f, 0.f);
        if (boxMode == mode::HORIZONTAL)
            {
                boxPositionIncrement.x = (0 * 2.f);
            }
        else
            {
                boxPositionIncrement.y = (0 * 2.f);
            }

        m_boxPositionIncrement = boxPositionIncrement;

        for (int i = 0; i < count; i++)
            {
                float posX = boxPositionIncrement.x * i;
                float posY = boxPositionIncrement.y * i;

                m_checkBoxes.emplace_back(checkBoxSize, 10.f, 5.f);
                m_checkBoxes.back().setPosition(posX, posY);

                if (m_optionCount >= i)
                    {
                        m_options.emplace_back(font);
                        m_options.back().setText(*(options.begin() + i));
                        m_options.back().setPosition(130.f, checkBoxSize + (checkBoxSize / 2.f));
                    }
            }
    }

void fe::gui::checkList::init(fe::gui::guiGraph &graph, int node)
    {
        for (int i = 0; i < m_checkBoxes.size(); i++)
            {
                int box = graph.addObjectToGraph(&m_checkBoxes[i], node);
                if (m_optionCount >= i)
                    {
                        graph.addObjectToGraph(&m_options[i], box);
                    }
            }
    }
