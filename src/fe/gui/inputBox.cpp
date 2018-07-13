#include "fe/gui/inputBox.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Glyph.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "fe/math/matrix.hpp"
#include "fe/gui/guiGraph.hpp"

void fe::gui::inputBox::handleOperator(char c)
	{
		if (c == '\b')
			{
				// backspace
			}
	}

void fe::gui::inputBox::handleCharacter(char c)
	{
		m_inputText += c;
	}

void fe::gui::inputBox::handleASCII(char c)
	{
		if (c < ' ')
			{
				handleOperator(c);
			}
		else if ((c >= '0' && c <= '9' && m_allowNumeric) || (m_allowAlpha))
			{
				
				handleCharacter(c);
			}
	}

void fe::gui::inputBox::handleWindowEvent(const sf::Event &event)
	{
		switch (event.type)
			{
				case sf::Event::TextEntered:
					handleASCII(event.text.unicode);
					break;
				default:
					break;
			}
	}

void fe::gui::inputBox::onStateChange(dialogStates previous, dialogStates next)
	{
		if (next == fe::gui::dialogStates::PRESSED)
			{
				m_active = !m_active;
			}
	}

fe::gui::inputBox::inputBox(const sf::Font *const font, modes mode) :
	m_font(font),
	m_text(font),
	m_modes(mode),
	m_allowAlpha(false),
	m_allowNumeric(false),
	m_active(false)
	{
	}

void fe::gui::inputBox::setSize(float x, float y)
	{
		m_bounds.x = x;
		m_bounds.y = y;
	}

void fe::gui::inputBox::setSize(fe::Vector2d size)
	{
		setSize(size.x, size.y);
	}

void fe::gui::inputBox::init(fe::gui::guiGraph &graph, int node)
	{
		graph.addObjectToGraph(&m_text, node);

		m_allowAlpha = static_cast<bool>(m_modes & modes::ALPHA);
		m_allowNumeric = static_cast<bool>(m_modes & modes::NUMERIC);

	}

fe::gui::inputBox::modes fe::gui::operator|(fe::gui::inputBox::modes lhs, fe::gui::inputBox::modes rhs)
	{
		return lhs | rhs;
	}

fe::gui::inputBox::modes fe::gui::operator&(fe::gui::inputBox::modes lhs, fe::gui::inputBox::modes rhs)
	{
		return lhs & rhs;
	}
