// dialogStates.hpp
// all states a dialog can be in
#pragma once

namespace fe
    {
        namespace gui
            {
                enum class dialogStates : int
                    {
                        NONE =          0,
                        ACTIVE =        1 << 0,
                        DISABLED =      1 << 1,
                        HIGHLIGHTED =   1 << 2,
                        PRESSED =       1 << 3,
                    };
            }
    }
