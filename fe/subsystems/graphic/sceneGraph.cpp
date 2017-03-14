#include "sceneGraph.hpp"
#include "../../entity/baseEntity.hpp"

#include <algorithm>

void fe::sceneGraph::update(float deltaTime)
    {
        for (auto &ent : m_entities)
            {
                ent->update(deltaTime);
            }
    }

void fe::sceneGraph::addEntity(fe::baseEntity *ent)
    {
        m_entities.push_back(ent);
    }

void fe::sceneGraph::removeEntity(fe::baseEntity *ent)
    {
        m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), ent), m_entities.end());
    }

void fe::sceneGraph::draw(sf::RenderWindow &app)
    {
        for (auto &ent : m_entities)
            {
                ent->draw(app);
            }
    }
