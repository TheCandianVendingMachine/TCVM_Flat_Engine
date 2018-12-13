#include "fe/subsystems/ai/graphNavigation.hpp"
#include "fe/subsystems/ai/graph.hpp"
#include "fe/subsystems/ai/node.hpp"
#include <queue>
#include <memory>
#include <algorithm>

void fe::graphNav::aStar(std::vector<int> &&waypoints, const graph &graph, int start, int end, float alpha)
    {
        if (start == end)
            {
                waypoints.push_back(end);
                return;
            }

        std::vector<unsigned int> open;
        std::vector<unsigned int> closed;

        open.push_back(start);
        int current = start;
        ai::node *endNode = graph.getNode(end);

        const float H_COST_MOD = 1.f;

        float dx = abs(graph.getNode(current)->m_posX - endNode->m_posX);
        float dy = abs(graph.getNode(current)->m_posY - endNode->m_posY);
        graph.getNode(current)->m_fCost = (1.f + alpha * (graph.getNode(end)->m_gCost - 1.f)) + graph.getNode(end)->m_hCost;

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

                if (current != end)
                    {
                        open.erase(std::remove(open.begin(), open.end(), current), open.end());
                        closed.push_back(current);

                        for (auto &neighbor : graph.getNode(current)->m_connectedNodes)
                            {
                                if (std::find(closed.begin(), closed.end(), neighbor) == closed.end())
                                    {
                                        float dx = abs(graph.getNode(neighbor)->m_posX - endNode->m_posX);
                                        float dy = abs(graph.getNode(neighbor)->m_posY - endNode->m_posY);
                                        float newCost = graph.getNode(current)->m_gCost + (std::sqrt(dx * dx + dy * dy) * H_COST_MOD);

                                        bool inOpenList = std::find(open.begin(), open.end(), neighbor) != open.end();
                                        if (!inOpenList)
                                            {
                                                open.push_back(neighbor);
                                            }
                                        else if (newCost >= graph.getNode(neighbor)->m_hCost)
                                            {   
                                                continue;
                                            }

                                        graph.getNode(neighbor)->m_hCost = newCost;
                                        graph.getNode(neighbor)->m_parent = current;
                                        graph.getNode(neighbor)->m_fCost = (1.f + alpha * (graph.getNode(neighbor)->m_gCost - 1.f)) + graph.getNode(neighbor)->m_hCost;
                                    }
                        }
                }
            }
        
        waypoints.push_back(end);
        ai::node *currentNode = endNode;
        while (currentNode->m_parent >= 0 && currentNode->m_parent != start)
            {
                waypoints.push_back(currentNode->m_parent);
                currentNode = graph.getNode(currentNode->m_parent);
            }
        waypoints.push_back(start);
        std::reverse(waypoints.begin(), waypoints.end());
    }
