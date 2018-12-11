#include "fe/subsystems/ai/graphNavigation.hpp"
#include "fe/subsystems/ai/graph.hpp"
#include "fe/subsystems/ai/node.hpp"
#include <queue>
#include <memory>
#include <algorithm>

void fe::graphNav::aStar(std::vector<int> &&waypoints, const graph &graph, int start, int end, float alpha)
    {
        std::vector<unsigned int> open;
        std::vector<unsigned int> closed;

        open.push_back(start);
        int current = start;
        ai::node *endNode = graph.getNode(end);

        const float H_COST_MOD = 1.f;

        while (!open.empty() && current != end)
            {
                float lowestFCost = std::numeric_limits<float>::max();
                for (auto &node : open)
                    {
                        if (graph.getNode(node)->m_fCost < lowestFCost)
                            {
                                lowestFCost = graph.getNode(node)->m_fCost;
                                current = node;
                            }
                    }

                open.erase(std::remove(open.begin(), open.end(), current), open.end());
                closed.push_back(current);

                for (auto &neighbor : graph.getNode(current)->m_connectedNodes)
                    {
                        if (std::find(closed.begin(), closed.end(), neighbor) == closed.end())
                            {
                                bool inOpenList = std::find(open.begin(), open.end(), neighbor) != open.end();
                                if (!inOpenList)
                                    {
                                        open.push_back(neighbor);
                                        graph.getNode(neighbor)->m_parent = current;
                                    }

                                float dx = abs(graph.getNode(neighbor)->m_posX - endNode->m_posX);
                                float dy = abs(graph.getNode(neighbor)->m_posY - endNode->m_posY);
                                graph.getNode(neighbor)->m_hCost = std::sqrt(dx * dx + dy * dy) * H_COST_MOD;

                                if (inOpenList && graph.getNode(neighbor)->m_gCost < graph.getNode(current)->m_gCost)
                                    {
                                        graph.getNode(neighbor)->m_parent = current;
                                    }

                                graph.getNode(neighbor)->m_fCost = (1.f + alpha * (graph.getNode(neighbor)->m_gCost - 1.f)) + graph.getNode(neighbor)->m_hCost;
                            }
                    }
            }
        
        waypoints.push_back(end);
        ai::node *currentNode = endNode;
        while (currentNode->m_parent >= 0)
            {
                waypoints.push_back(currentNode->m_parent);
                currentNode = graph.getNode(currentNode->m_parent);
            }
        std::reverse(waypoints.begin(), waypoints.end());
    }
