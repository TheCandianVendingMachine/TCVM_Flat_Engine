#include "fe/gui/guiBatch.hpp"
#include "fe/math/polygon2.hpp"
#include "fe/subsystems/physics/transformable.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::gui::guiBatch::guiBatch()
    {
        clear();
    }

void fe::gui::guiBatch::add(const polygon2d &polygon, sf::Color drawColour, const fe::matrix3d &drawMatrix)
    {
        for (auto &triangle : polygon.m_verticies)
            {
                if (m_batch.size() < (m_vertexIndex + 3))
                    {
                        m_batch.emplace_back();
                        m_batch.emplace_back();
                        m_batch.emplace_back();
                    }

                m_batch[m_vertexIndex + 0].position = fe::Vector2d(drawMatrix.transformPoint(std::forward<const fe::lightVector2d>(triangle[0]))).convertToSfVec2();
                m_batch[m_vertexIndex + 1].position = fe::Vector2d(drawMatrix.transformPoint(std::forward<const fe::lightVector2d>(triangle[1]))).convertToSfVec2();
                m_batch[m_vertexIndex + 2].position = fe::Vector2d(drawMatrix.transformPoint(std::forward<const fe::lightVector2d>(triangle[2]))).convertToSfVec2();

                m_batch[m_vertexIndex + 0].color = drawColour;
                m_batch[m_vertexIndex + 1].color = drawColour;
                m_batch[m_vertexIndex + 2].color = drawColour;

                m_vertexIndex += 3;
                m_vertexCount += 3;
            }
    }

void fe::gui::guiBatch::clear()
    {
        m_vertexIndex = 0;
        m_vertexCount = 0;
    }

void fe::gui::guiBatch::draw(sf::RenderTarget &app, sf::RenderStates states)
    {
        app.draw(m_batch.data(), m_vertexCount, sf::PrimitiveType::Triangles, states);
    }
