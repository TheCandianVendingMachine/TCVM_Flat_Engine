// graphNavigation.hpp
// Generates waypoints to navigate through a graph
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

#include <vector>
namespace fe
	{
		class graph;

		namespace graphNav 
			{
				FLAT_ENGINE_API std::vector<unsigned int> aStar(graph &graph, unsigned int start, unsigned int end, float alpha = 1.f);

			}
	}