// graph.hpp
// graph class. Is a basic graph
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

#include <vector>
#include <memory>

namespace fe
	{
		class graph;
		namespace graphNav
			{
				std::vector<int> aStar(graph&, int, int, float);
			}

		class graph
			{
				private:
					struct node
						{
							// Nodes this node is connected to
							std::vector<int> m_connectedNodes;
							int m_parent; // used for pathfinding

							float m_gCost;
							float m_hCost;
							float m_fCost;

							float m_posX;
							float m_posY;

							node() : m_parent(-1), m_gCost(-1.f), m_hCost(-1.f), m_fCost(-1.f), m_posX(0.f), m_posY(0.f) {}
						};

					std::vector<std::unique_ptr<node>> m_nodes;
					std::vector<int> m_nodeHandles;

					friend std::vector<int> graphNav::aStar(graph&, int, int, float);

				public:
					FLAT_ENGINE_API int addNode(float posX, float posY, float cost = 1.f);

                    FLAT_ENGINE_API void addEdge(int nodeA, int nodeB);
                    FLAT_ENGINE_API void removeEdge(int nodeID, int edge);
                    FLAT_ENGINE_API node *getNode(int nodeID);
                    FLAT_ENGINE_API const std::vector<int> &getAllNodes();

			};
	}