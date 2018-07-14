// inputBoxModes.hpp
// all input box modes
#pragma once

namespace fe
	{
		namespace gui
			{
				enum class inputBoxModes : int
					{
						NONE				= 0,
						DISALLOW_ALPHA		= 1 << 0,
						DISALLOW_NUMERIC	= 1 << 1,
						STOP_WHEN_FULL		= 1 << 2,
					};

				inline inputBoxModes operator|(inputBoxModes lhs, inputBoxModes rhs) { return static_cast<fe::gui::inputBoxModes>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
				inline inputBoxModes operator&(inputBoxModes lhs, inputBoxModes rhs) { return static_cast<fe::gui::inputBoxModes>(static_cast<int>(lhs) & static_cast<int>(rhs)); }
			}
	}
