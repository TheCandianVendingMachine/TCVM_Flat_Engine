#include "sceneGraph.hpp"
#include "../../entity/transformable.hpp"
#include "../../entity/baseEntity.hpp"
#include "../../entity/drawable.hpp"

#include "../../debug/logger.hpp"

#include <algorithm>

void fe::sceneGraph::update(float deltaTime)
    {
        m_batch.clear();
        for (auto &ent : getObjects())
            {
                ent->update(deltaTime);
                ent->draw(m_batch, *ent);
            }
    }

void fe::sceneGraph::postUpdate()
    {
        for (auto &ent : getObjects())
            {
                if (ent->hasCollider())
                    {
                        ent->getCollider().m_position = ent->getPosition();
                    }
            }
    }

void fe::sceneGraph::draw(sf::RenderTarget &app)
    {
        m_batch.draw(app);
    }

fe::sceneGraph::~sceneGraph()
    {
        for (auto &ent : getObjects())
            {
                delete ent;
            }
    }
