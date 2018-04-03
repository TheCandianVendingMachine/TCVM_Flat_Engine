// mathFuncs.hpp
// various mathematical functions
#pragma once

namespace fe
    {
        template <typename T>
        int signOf(const T &number)
            {
                return (T(0) < number) - (number < T(0));
            }
    }

