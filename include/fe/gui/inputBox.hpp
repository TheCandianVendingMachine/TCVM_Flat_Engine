// inputBox.hpp
// A box that when selected the user can type into
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/gui/control.hpp"
#include "fe/gui/text.hpp"
#include "fe/math/Vector2.hpp"
#include <string>

namespace sf
	{
		class Font;
		class RenderTarget;
	}

namespace fe
	{
		class transformable;

		namespace gui
			{
				class inputBox : public fe::gui::control
					{
						public:
							enum class modes : int
								{
									NONE = 0,
									ALPHA = 1 << 0,
									NUMERIC = 1 << 1
								};

							friend modes operator|(modes lhs, modes rhs);
							friend modes operator&(modes lhs, modes rhs);

						private:
							fe::gui::text m_text;
							const sf::Font *const m_font;

							fe::Vector2d m_bounds;

							std::string m_inputText;
							std::string m_inputTextDisplayed;

							modes m_modes;

							bool m_allowAlpha;
							bool m_allowNumeric;

							bool m_active;

							FLAT_ENGINE_API void handleOperator(char c);
							FLAT_ENGINE_API void handleCharacter(char c);
							FLAT_ENGINE_API void handleASCII(char c);
							FLAT_ENGINE_API void handleWindowEvent(const sf::Event &event);
							FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next);

						public:
							FLAT_ENGINE_API inputBox(const sf::Font *const font, modes mode = modes::ALPHA | modes::NUMERIC);
							FLAT_ENGINE_API void setSize(float x, float y);
							FLAT_ENGINE_API void setSize(fe::Vector2d size);

							FLAT_ENGINE_API void init(fe::gui::guiGraph &graph, int node);
					};
			}
	}
