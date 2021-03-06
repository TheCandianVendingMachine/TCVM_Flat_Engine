#include "fe/gui/checkList.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/gui/guiGraph.hpp"

void fe::gui::checkList::onStateChange(dialogStates previous, dialogStates next)
    {
        
    }

void fe::gui::checkList::drawDialogElements(sf::RenderTarget &target, const fe::transformable &drawMatrix)
    {
        FE_ENGINE_PROFILE("gui_check_list", "draw");
        drawPolygon(getControlPolygon(), const_cast<fe::transformable&>(drawMatrix).getMatrix(), getDrawColour());
        drawToScreen(target, drawMatrix);
        FE_END_PROFILE;
    }

void fe::gui::checkList::handleWindowEvent(const sf::Event &event)
    {
        for (auto &box : m_checkBoxes)
            {
                box.handleEvent(event);
            }
    }

fe::gui::checkList::checkList(
    float checkBoxSize, unsigned int count,
    float gapBetweenBoxes, float boxOutlineWidth, float checkMarkWidthFromSide,
    const sf::Font *font, std::initializer_list<std::string> options, unsigned int charSize, float gapBetweenTextAndOption
) :
    m_boxSize(checkBoxSize),
    m_gapBetweenBoxes(gapBetweenBoxes),
    m_optionCount((int)options.size() - 1)
    {
        addPoint(0.f, 0.f);
        addPoint(20.f, 0.f);
        addPoint(20.f, 20.f);
        addPoint(0.f, 20.f);
        //getControlPolygon().createPolygon();

        for (int i = 0; i < count; i++)
            {
                m_checkBoxes.emplace_back(m_boxSize, boxOutlineWidth, checkMarkWidthFromSide);

                if (m_optionCount >= i)
                    {
                        m_options.emplace_back(font);
                        m_options.back().setText(*(options.begin() + i));
                    }
            }

        updateBoxes(checkBoxSize, gapBetweenBoxes, boxOutlineWidth, checkMarkWidthFromSide, charSize, gapBetweenTextAndOption);
    }

void fe::gui::checkList::updateBoxes(float checkBoxSize, float gapBetweenBoxes, float boxOutlineWidth, float checkMarkWidthFromSide, unsigned int charSize, float gapBetweenTextAndBox)
    {
        m_boxSize = checkBoxSize;
        m_gapBetweenBoxes = gapBetweenBoxes;

        fe::lightVector2d boxPositionIncrement(0.f, (m_boxSize)+(boxOutlineWidth * 2.f) + (gapBetweenBoxes));

        for (int i = 0; i < m_checkBoxes.size(); i++)
            {
                m_checkBoxes[i].createButton(m_boxSize, boxOutlineWidth, checkMarkWidthFromSide);
                m_checkBoxes[i].setPosition(0.f, boxPositionIncrement.y * i);

                if (i < m_options.size())
                    {
                        m_options[i].setCharacterSize(charSize);
                        m_options[i].setPosition((m_boxSize) + (boxOutlineWidth * 2.f) + (m_options[i].getSize().x / 2.f) + gapBetweenTextAndBox, (m_boxSize / 2.f) + boxOutlineWidth);
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

fe::gui::checkBox &fe::gui::checkList::getCheckbox(unsigned int index)
    {
        return m_checkBoxes[index];
    }

void fe::gui::checkList::getSelected(std::vector<bool> &selected) const
    {
        for (unsigned int i = 0; i < m_checkBoxes.size(); i++)
            {
                if (i < selected.size())
                    {
                        selected[i] = m_checkBoxes[i].isSelected();
                    }
                else
                    {
                        selected.push_back(m_checkBoxes[i].isSelected());
                    }
            }
    }

std::vector<bool> fe::gui::checkList::getSelected() const
    {
        std::vector<bool> returnVec;
        getSelected(returnVec);
        return returnVec;
    }

bool fe::gui::checkList::anySelected() const
    {
        for (auto &selection : m_checkBoxes)
            {
                if (selection.isSelected())
                    {
                        return true;
                    }
            }
        return false;
    }

fe::lightVector2d fe::gui::checkList::getSize() const
    {
        float size = ((m_gapBetweenBoxes + m_boxSize) * m_optionCount) - m_gapBetweenBoxes; // subtract gapBetweenSelections to avoid extra space on the bottom

        fe::lightVector2d min = fe::Vector2d() - fe::lightVector2d(m_boxSize, m_boxSize);
        fe::lightVector2d max(size, size);

        for (auto &text : m_options)
            {
                min.x = std::min(min.x, text.getSize().x);
                max.x = std::max(max.x, text.getSize().x);

                min.y = std::min(min.y, text.getSize().y);
                max.y = std::max(max.y, text.getSize().y);
            }

        return max - min;
    }
