// alignment.hpp
// an enum to control the alignment of GUI elements
#pragma once

namespace fe
	{
		namespace gui
			{
				enum class align : unsigned char
					{
						MIDDLE			= 0,
						TOP_LEFT		= (1 << 0),
						TOP_MIDDLE		= (1 << 1),
						TOP_RIGHT		= (1 << 2),
						RIGHT			= (1 << 3),
						BOTTOM_RIGHT	= (1 << 4),
						BOTTOM_MIDDLE	= (1 << 5),
						BOTTOM_LEFT		= (1 << 6),
						LEFT			= (1 << 7)
					};
			}
	}
