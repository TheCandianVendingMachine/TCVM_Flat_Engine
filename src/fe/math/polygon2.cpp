#include "fe/math/polygon2.hpp"
#include "fe/math/Vector3.hpp"
#include "fe/dataStructures/doublyLinkedList.hpp"

bool triangleIsCCW(fe::Vector2d a, fe::Vector2d p, fe::Vector2d c)
    {
        // If cross product of abc is > 0 its ccw
        float crossP = (p[0] - a[0]) * (c[1] - a[1]) - (p[1] - a[1]) * (c[0] - a[0]);
        return crossP >= 0;
    }

bool pointInTriangle(fe::Vector2d p, fe::Vector2d a, fe::Vector2d b, fe::Vector2d c)
    {
        if ((p - a).cross(b - a) > 0.f) return false;
        if ((p - b).cross(c - b) > 0.f) return false;
        if ((p - c).cross(a - c) > 0.f) return false;

        return true;
    }

fe::polygon2d::polygon2d(const std::initializer_list<fe::lightVector2d> &points)
    {
        createPolygon(points);
    }

void fe::polygon2d::addPoint(fe::lightVector2d point)
    {
        m_points.push_back(point);
    }

void fe::polygon2d::createPolygon(const std::initializer_list<fe::lightVector2d> &points)
    {
        m_verticies.clear();
        m_points.clear();
        for (auto &point : points)
            {
                addPoint(point);
            }
        createPolygon();
    }

void fe::polygon2d::createPolygon()
    {
        if (m_points.size() < 2) return;

        // Polygon Triangulation using ear clipping algorithm
        // Source: Real Time Collision Detection
        
        int size = m_points.size();

        std::vector<int> prev;
        std::vector<int> next;
        for (int i = 0; i < m_points.size(); i++)
            {
                prev.push_back(i - 1);
                next.push_back(i + 1);
            }
        prev[0] = size - 1;
        next[size - 1] = 0;

        int i = 0;
        while (size > 3)
            {
                bool isEar = true;
                if (triangleIsCCW(m_points[prev[i]], m_points[i], m_points[next[i]]))
                    {
                        int k = next[next[i]];
                        do
                        {
                            bool isSamePoint = 
                                (m_points[k].x == m_points[prev[i]].x   && m_points[k].y == m_points[prev[i]].y)    ||
                                (m_points[k].x == m_points[i].x         && m_points[k].y == m_points[i].y)          ||
                                (m_points[k].x == m_points[next[i]].x   && m_points[k].y == m_points[next[i]].y);

                            if (!isSamePoint && pointInTriangle(m_points[k], m_points[prev[i]], m_points[i], m_points[next[i]]))
                                {
                                    isEar = false;
                                    break;
                                }
                            k = next[k];
                        } while (k != prev[i]);
                    }
                else
                    {
                        isEar = false;
                    }

                if (isEar)
                    {
                        m_verticies.push_back({ m_points[prev[i]], m_points[i], m_points[next[i]] });

                        next[prev[i]] = next[i];
                        prev[next[i]] = prev[i];
                        size--;

                        i = prev[i];
                    }
                else
                    {
                        i = next[i];
                    }
            }

        m_verticies.push_back({ m_points[prev[i]], m_points[i], m_points[next[i]] });
    }

bool fe::polygon2d::pointInPolygon(fe::lightVector2d point)
    {
        for (auto &triangle : m_verticies)
            {
                if (pointInTriangle(point, triangle[0], triangle[1], triangle[2]))
                    {
                        return true;
                    }
            }
        return false;
    }

bool fe::polygon2d::pointInPolygon(float x, float y)
    {
        return pointInPolygon({ x, y });
    }

void fe::polygon2d::clear()
    {
        m_verticies.clear();
        m_points.clear();
    }
