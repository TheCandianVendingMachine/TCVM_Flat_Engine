#include "fe/gui/inputBox.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Glyph.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "fe/math/matrix.hpp"
#include "fe/gui/guiGraph.hpp"
#include "fe/debug/debugDraw.hpp"

void fe::gui::inputBox::fitText()
	{
		while (m_textBounds.x >= m_bounds.x - (m_containerOutlineWidth.x * 2.f))
			{
				if (m_scrollWhenFull)
					{
						m_textBounds.x -= getCharacterSize(*(m_inputTextDisplayed.begin())).x;
						m_inputTextDisplayed.erase(m_inputTextDisplayed.begin());
					}
				else
					{
						m_textBounds.x -= getCharacterSize(*(m_inputTextDisplayed.end() - 1)).x;
						m_inputText.pop_back();
						m_inputTextDisplayed.pop_back();
					}
			}
	}

fe::Vector2d fe::gui::inputBox::getCharacterSize(char c)
	{
		const sf::Glyph &glyph = m_font->getGlyph(c, m_text.getCharacterSize(), false, 0.f);
		return fe::Vector2d(glyph.advance, glyph.bounds.height);
	}

void fe::gui::inputBox::handleOperator(char c)
	{
		if (c == '\b')
			{
				// backspace
				if (!m_inputText.empty()) 
					{
						m_inputText.pop_back();

						if (m_inputTextDisplayed.size() <= m_inputText.size())
							{
								char potChar = *(m_inputText.end() - m_inputTextDisplayed.size());
								m_inputTextDisplayed.insert(m_inputTextDisplayed.begin(), potChar);

								m_textBounds.x += getCharacterSize(potChar).x;
							}

						m_textBounds.x -= getCharacterSize(*(m_inputTextDisplayed.end() - 1)).x;
						m_inputTextDisplayed.pop_back();
					}

				fitText();
			}
	}

void fe::gui::inputBox::handleCharacter(char c)
	{
		m_inputText += c;
		m_inputTextDisplayed += c;

		fe::Vector2d size = getCharacterSize(c);
		m_textBounds.x += size.x;
		m_textBounds.y = std::max(m_textBounds.y, size.y);
		
		fitText();
	}

void fe::gui::inputBox::handleASCII(char c)
	{
		if (c < ' ')
			{
				handleOperator(c);
			}
		else if ((m_allowNumeric && (c >= '0' && c <= '9' || c == '.' || c == '-')) || (m_allowAlpha && (c < '0' || c > '9')))
			{
				handleCharacter(c);
			}

		m_text.setText(m_inputTextDisplayed);
		m_text.setPosition(m_containerOutlineWidth.x + m_textDistanceFromEdge, m_containerOutlineWidth.y + (m_bounds.y / 2.f), fe::gui::align::LEFT);
	}

void fe::gui::inputBox::handleWindowEvent(const sf::Event &event)
	{
		switch (event.type)
			{
				case sf::Event::TextEntered:
					if (m_active) 
						{
							handleASCII(event.text.unicode);
						}
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

void fe::gui::inputBox::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
	{
		//FE_DEBUG_DRAW_SQUARE(m_textBounds.x, m_textBounds.y, m_text.getPosition().x + getPosition().x, m_text.getPosition().y + getPosition().y, sf::Color::Red);
		drawPolygon(m_textContainer, drawMatrix, getDrawColour());
		draw(target);
	}

fe::gui::inputBox::inputBox(const sf::Font *const font, modes mode) :
	m_font(font),
	m_text(font),
	m_modes(mode),
	m_allowAlpha(true),
	m_allowNumeric(true),
	m_scrollWhenFull(true),
	m_active(false),
	m_containerOutlineWidth(5.f, 5.f),
	m_textDistanceFromEdge(5.f)
	{
	}

void fe::gui::inputBox::setSize(float x, float y)
	{
		m_bounds.x = x;
		m_bounds.y = y;

		setOutlineSize(m_containerOutlineWidth);

		getControlPolygon().clear();
		getControlPolygon().addPoint(0.f, 0.f);
		getControlPolygon().addPoint(x, 0.f);
		getControlPolygon().addPoint(x, y);
		getControlPolygon().addPoint(0.f, y);
		getControlPolygon().createPolygon();

		m_text.setPosition(m_containerOutlineWidth.x + m_textDistanceFromEdge, m_bounds.y / 2.f, fe::gui::align::BOTTOM_LEFT);
	}

void fe::gui::inputBox::setSize(fe::Vector2d size)
	{
		setSize(size.x, size.y);
	}

void fe::gui::inputBox::setOutlineSize(float size)
	{
		setOutlineSize(size, size);
	}

void fe::gui::inputBox::setOutlineSize(fe::Vector2d size)
	{
		setOutlineSize(size.x, size.y);
	}

void fe::gui::inputBox::setOutlineSize(float x, float y)
	{
		m_textContainer.clear();

		m_containerOutlineWidth.x = x;
		m_containerOutlineWidth.y = y;
		
		m_textContainer.addPoint(0.f, 0.f);
		m_textContainer.addPoint(m_bounds.x + m_containerOutlineWidth.x, 0.f);
		m_textContainer.addPoint(m_bounds.x + m_containerOutlineWidth.x, m_bounds.y + m_containerOutlineWidth.y);
		m_textContainer.addPoint(0.f, m_bounds.y + m_containerOutlineWidth.y);

		m_textContainer.addPoint(0.f, 0.f);

		m_textContainer.addPoint(m_containerOutlineWidth.x, m_containerOutlineWidth.y);
		m_textContainer.addPoint(m_containerOutlineWidth.x, m_bounds.y);
		m_textContainer.addPoint(m_bounds.x, m_bounds.y);
		m_textContainer.addPoint(m_bounds.x, m_containerOutlineWidth.y);
		
		m_textContainer.addPoint(m_containerOutlineWidth.x, m_containerOutlineWidth.y);

		m_textContainer.createPolygon();
	}

void fe::gui::inputBox::init(fe::gui::guiGraph &graph, int node)
	{
		setSize(100.f, 50.f);

		graph.addObjectToGraph(&m_text, node);
		m_text.setDrawColour(sf::Color::White);
		setDrawColour(sf::Color::White);

		m_allowAlpha = !static_cast<bool>(m_modes & modes::DISALLOW_ALPHA);
		m_allowNumeric = !static_cast<bool>(m_modes & modes::DISALLOW_NUMERIC);
		m_scrollWhenFull = !static_cast<bool>(m_modes & modes::STOP_WHEN_FULL);
	}

std::string fe::gui::inputBox::getInput() const
	{
		return m_inputText;
	}

std::string fe::gui::inputBox::getDisplayedInput() const
	{
		return m_inputTextDisplayed;
	}

fe::gui::inputBox::modes fe::gui::operator|(fe::gui::inputBox::modes lhs, fe::gui::inputBox::modes rhs)
	{
		return static_cast<fe::gui::inputBox::modes>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

fe::gui::inputBox::modes fe::gui::operator&(fe::gui::inputBox::modes lhs, fe::gui::inputBox::modes rhs)
	{
		return static_cast<fe::gui::inputBox::modes>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}
