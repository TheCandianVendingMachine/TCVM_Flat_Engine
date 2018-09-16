// particleFlags.hpp
// Flags for each particle
#pragma once
#include <cstdint>

namespace fe
    {
        enum class particleFlags : std::int16_t
            {
                NONE = 0,
                KILLED = 1 << 0,
                IGNORE_COLLISIONS = 1 << 1,
            };

        inline constexpr particleFlags operator | (particleFlags lhs, particleFlags rhs)
            {
                return static_cast<particleFlags>(static_cast<std::int16_t>(lhs) | static_cast<std::int16_t>(rhs));
            }

        inline constexpr int16_t operator & (particleFlags lhs, particleFlags rhs)
            {
                 return static_cast<std::int16_t>(lhs) & static_cast<std::int16_t>(rhs);
            }

    }

