// engineEvents.hpp
// Events that are defined in the engine. All other events will have the offset of the size of this applied to them
#pragma once

namespace fe
    {
        enum engineEvent
            {
                GUI_PANEL_MOVED = 1,
                GUI_PANEL_MINIMIZED,
                GUI_PANEL_CLOSED,
                GUI_PANEL_SIZE_CHANGE,
                TILE_PLACED,
                TILE_REMOVED,
                COUNT
        };
    }