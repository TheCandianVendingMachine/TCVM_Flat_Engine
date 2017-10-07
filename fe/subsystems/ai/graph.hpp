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
				std::vector<unsigned int> aStar(graph&, unsigned int, unsigned int, float);
			}

		class graph
			{
				private:
					struct node
						{
							// Nodes this node is connected to
							std::vector<unsigned int> m_connectedNodes;
							int m_parent; // used for pathfinding

							float m_gCost;
							float m_hCost;
							float m_fCost;

							float m_posX;
							float m_posY;

                            void *m_userData;

							node() : m_parent(-1), m_gCost(-1.f), m_hCost(-1.f), m_fCost(-1.f), m_posX(0.f), m_posY(0.f) {}
						};

					std::vector<std::unique_ptr<node>> m_nodes;
					std::vector<unsigned int> m_nodeHandles;

					friend std::vector<unsigned int> graphNav::aStar(graph&, unsigned int, unsigned int, float);

				public:
					FLAT_ENGINE_API int addNode(float posX, float posY, float cost = 1.f);

                    FLAT_ENGINE_API void addEdge(unsigned int nodeA, unsigned int nodeB);
                    FLAT_ENGINE_API void removeEdge(unsigned int nodeID, unsigned int edge);
                    FLAT_ENGINE_API node *getNode(unsigned int nodeID);
                    FLAT_ENGINE_API const std::vector<unsigned int> &getAllNodes();

			};
	}