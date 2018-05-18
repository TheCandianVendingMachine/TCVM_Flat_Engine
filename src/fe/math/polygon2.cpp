#include "fe/math/polygon2.hpp"
#include "fe/math/Vector3.hpp"
#include "fe/dataStructures/doublyLinkedList.hpp"

bool isPointConvex(fe::Vector2d a, fe::Vector2d p, fe::Vector2d c, fe::Vector2d center)
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

bool triangleIsCCW(fe::lightVector2d a, fe::lightVector2d b, fe::lightVector2d c)
    {
        float det = ((a.x - c.x) * (b.y - c.y)) - ((a.y - c.y) * (b.x - c.x));
        return det > 0.f;
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
        // https://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf
        fe::doublyLinkedList<fe::lightVector2d> polygon;
        using polyNode = fe::doublyLinkedList<fe::lightVector2d>::node;
        fe::Vector2d center;

        for (auto &point : m_points)
            {
                polygon.insert(point);
                center += point;
            }

        center /= polygon.size();

        unsigned int index = 0;
        while (polygon.size() > 2)
            {
                polyNode *a = nullptr;
                polyNode *p = nullptr;
                polyNode *c = nullptr;

                if (index != 0) 
                    {
                        a = polygon.at(index - 1);
                    }
                else
                    {
                        a = polygon.at(polygon.size() - 1);
                    }

                p = polygon.at(index);

                if (index + 1 < polygon.size())
                    {
                        c = polygon.at(index + 1);
                    }
                else
                    {
                        c = polygon.at(0);
                    }
                
                if (isPointConvex(a->m_data, p->m_data, c->m_data, center))
                    {
                        for (unsigned int i = 0; i < polygon.size(); i++)
                            {
                                polyNode *indexNode = polygon.at(i);
                                if (indexNode != a && indexNode != p && indexNode != c)
                                    {
                                        if (pointInTriangle(indexNode->m_data, a->m_data, p->m_data, c->m_data))
                                            {
                                                goto exit_polygon_check;
                                            }
                                    }
                            }

                        m_verticies.push_back({ a->m_data, p->m_data, c->m_data });
                        polygon.remove(p);

                        exit_polygon_check:;
                    }

                index++;
                if (index >= polygon.size())
                    {
                        index = 0;
                    }
            }
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
