#include "fe/gui/radioList.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/gui/guiGraph.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/messaging/gameEvent.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"

void fe::gui::radioList::onStateChange(dialogStates previous, dialogStates next)
    {
    
    }

void fe::gui::radioList::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        FE_ENGINE_PROFILE("gui_radio_list", "draw");
        drawPolygon(getControlPolygon(), drawMatrix, getDrawColour());
        draw(target);
        FE_END_PROFILE;
    }

void fe::gui::radioList::handleWindowEvent(const sf::Event &event)
    {
        for (auto &selection : m_selections)
            {
                selection.handleEvent(event);
            }
    }

fe::gui::radioList::radioList(
    float selectionSize, unsigned int count,
    float gapBetweenSelections, float selectionOutlineWidth, float markWidthFromSide,
    const sf::Font *font, std::initializer_list<std::string> options, unsigned int charSize, float gapBetweenTextAndOption) :
    m_selectionSize(selectionSize),
    m_optionCount((int)options.size() - 1),
    m_gapBetweenSelections(gapBetweenSelections)
    {
        addPoint(0.f, 0.f);
        addPoint(20.f, 0.f);
        addPoint(20.f, 20.f);
        addPoint(0.f, 20.f);
        //getControlPolygon().createPolygon();

        for (int i = 0; i < count; i++)
            {
                m_selections.emplace_back(i, eventHandlerId(), 30, m_selectionSize, selectionOutlineWidth, markWidthFromSide);

                if (m_optionCount >= i)
                    {
                        m_options.emplace_back(font);
                        m_options.back().setText(*(options.begin() + i));
                    }
            }

        updateSelections(selectionSize, gapBetweenSelections, selectionOutlineWidth, markWidthFromSide, charSize, gapBetweenTextAndOption);
    }

void fe::gui::radioList::updateSelections(float selectionSize, float gapBetweenSelections, float selectionOutlineWidth, float markWidthFromSide, unsigned int charSize, float gapBetweenTextAndBox)
    {
        m_selectionSize = selectionSize / 2.f;
        m_gapBetweenSelections = gapBetweenSelections;

        fe::lightVector2d boxPositionIncrement(0.f, (m_selectionSize * 2.f) + (selectionOutlineWidth * 2.f) + (m_gapBetweenSelections));

        for (int i = 0; i < m_selections.size(); i++)
            {
                m_selections[i].createButton(m_selectionSize, selectionOutlineWidth, markWidthFromSide);
                m_selections[i].setPosition(0.f, boxPositionIncrement.y * i);

                if (i < m_options.size())
                    {
                        m_options[i].setCharacterSize(charSize);
                        m_options[i].setPosition((m_selectionSize * 2.f) + (m_options[i].getSize().x / 2.f) + gapBetweenTextAndBox, m_selectionSize);
                    }
            }
    }

void fe::gui::radioList::init(fe::gui::guiGraph &graph, int node)
    {
        for (int i = 0; i < m_selections.size(); i++)
            {
                int box = graph.addObjectToGraph(&m_selections[i], node);
                if (m_optionCount >= i)
                    {
                        graph.addObjectToGraph(&m_options[i], box);
                    }
            }

        fe::engine::get().getEventSender().subscribe(this);
    }

fe::gui::radioButton &fe::gui::radioList::getSelection(unsigned int index)
    {
        return m_selections[index];
    }

void fe::gui::radioList::getSelected(std::vector<bool> &selected)
    {
        for (unsigned int i = 0; i < m_selections.size(); i++)
            {
                if (i < selected.size())
                    {
                        selected[i] = m_selections[i].isSelected();
                    }
                else
                    {
                        selected.push_back(m_selections[i].isSelected());
                    }
            }
    }

std::vector<bool> fe::gui::radioList::getSelected()
    {
        std::vector<bool> returnVec;
        getSelected(returnVec);
        return returnVec;
    }

void fe::gui::radioList::handleEvent(const gameEvent &event)
    {
        switch (event.sendType)
            {
                case fe::sendType::SEND_TO_TARGET:
                    for (auto &selection : m_selections)
                        {
                            selection.setSelected(false);
                        }
                    break;
                default:
                    break;
            }
    }

fe::lightVector2d fe::gui::radioList::getSize() const
	{
		float size = ((m_gapBetweenSelections + m_selectionSize) * m_optionCount) - m_gapBetweenSelections; // subtract gapBetweenSelections to avoid extra space on the bottom

		fe::lightVector2d min = getPosition();
		fe::lightVector2d max(size, size);

		for (auto &text : m_options)
			{
				// divide position by two to account for getting the text position from the middle
				min.x = std::min(min.x, (text.getPosition().x / 2.f) + text.getSize().x);
				max.x = std::max(max.x, (text.getPosition().x / 2.f) + text.getSize().x);

				min.y = std::min(min.y, (text.getPosition().y / 2.f) + text.getSize().y);
				max.y = std::max(max.y, (text.getPosition().y / 2.f) + text.getSize().y);
			}

		return max - min;
	}
