// graph.hpp
// graph class. Is a basic graph
#pragma once
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
					int addNode(float posX, float posY, float cost = 1.f);

					void addEdge(int nodeA, int nodeB);
					void removeEdge(int nodeID, int edge);
					node *getNode(int nodeID);
					const std::vector<int> &getAllNodes();

			};
	}