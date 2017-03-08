#include "sceneGraph.hpp"

void fe::sceneGraph::startUp()
    {
    }

void fe::sceneGraph::shutDown()
    {
    }

void fe::sceneGraph::postUpdate()
    {
        for (auto &drawable : m_drawables)
            {
                m_nextDrawable.push(drawable);
            }
    }

void fe::sceneGraph::addDrawable(sf::Drawable *draw)
    {
        m_drawables.push_back(draw);
    }

sf::Drawable *fe::sceneGraph::getNextDrawable()
    {
        if (!m_nextDrawable.empty()) 
            {
                auto next = m_nextDrawable.front();
                m_nextDrawable.pop();
                return next;
            }

        return nullptr;
    }
