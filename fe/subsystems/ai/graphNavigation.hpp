// graphNavigation.hpp
// Generates waypoints to navigate through a graph
#pragma once
#include <vector>
namespace fe
	{
		class graph;

		namespace graphNav 
			{
				std::vector<int> aStar(graph &graph, int start, int end, float alpha = 1.f);

			}
	}