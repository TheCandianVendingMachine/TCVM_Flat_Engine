#include "sceneGraph.hpp"
#include "../../entity/transformable.hpp"
#include "../../entity/baseEntity.hpp"
#include "../../entity/drawable.hpp"

#include "../../debug/profiler.hpp"

#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>

void fe::sceneGraph::update(float deltaTime)
    {
        m_batch.clear();
        for (auto &ent : m_entities)
            {
                ent->update(deltaTime);
                ent->draw(m_batch, *ent);
            }
    }

void fe::sceneGraph::postUpdate()
    {
    }

void fe::sceneGraph::addEntity(fe::baseEntity *ent)
    {
        m_entities.push_back(ent);
    }

void fe::sceneGraph::removeEntity(fe::baseEntity *ent)
    {
        m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), ent), m_entities.end());
    }

void fe::sceneGraph::draw(sf::RenderTarget &app)
    {
        m_batch.draw(app);
    }
