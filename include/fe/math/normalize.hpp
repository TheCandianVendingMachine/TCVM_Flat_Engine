// normalize.hpp
// Normalizes data in arbitrary range
#pragma once

namespace fe
	{
		// normalizes data between [0, 1]
		template<typename T>
		T normalize(T min, T max, T value)
			{
				return fe::normalize(min, max, value, T(0), T(1));
			}

		// Normalizes data between [minRange, maxRange]
		template<typename T>
		T normalize(T min, T max, T value, T minRange, T maxRange)
			{
				return (maxRange - minRange) * ((value - min) / (max - min)) + minRange;
			}
	}