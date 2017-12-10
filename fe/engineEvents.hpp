// engineEvents.hpp
// Events that are defined in the engine. All other events will have the offset of the size of this applied to them
#pragma once

namespace fe
    {
        enum engineEvent
            {
                GUI_PANEL_MOVED = 0,
                GUI_PANEL_MINIMIZED = 1 << 0,
                GUI_PANEL_CLOSED = 1 << 1,
                GUI_PANEL_SIZE_CHANGE = 1 << 2,
                COUNT = 5
        };
    }