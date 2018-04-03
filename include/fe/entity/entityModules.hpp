// entityModules.hpp
// Enum of all modules
#pragma once
#include <cstdint>

namespace fe
    {
        enum class entityModules : std::int16_t
            {
                NONE            = 0,
                RENDER_OBJECT   = 1 << 0,
                RENDER_TEXT     = 1 << 1,
                RIGID_BODY      = 1 << 2,
                COLLISION_BODY  = 1 << 3,
            };

        inline constexpr entityModules operator | (entityModules lhs, entityModules rhs)
            {
                return static_cast<entityModules>(static_cast<std::int16_t>(lhs) | static_cast<std::int16_t>(rhs));
            }

        inline constexpr int16_t operator & (entityModules lhs, entityModules rhs)
            {
                return static_cast<std::int16_t>(lhs) & static_cast<std::int16_t>(rhs);
            }
    }
