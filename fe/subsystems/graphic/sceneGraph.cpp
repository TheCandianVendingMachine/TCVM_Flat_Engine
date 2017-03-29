#include "sceneGraph.hpp"
#include "../../entity/transformable.hpp"
#include "../../entity/baseEntity.hpp"
#include "../../entity/drawable.hpp"

#include "../../debug/logger.hpp"

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
        for (auto &ent : m_entities)
            {
                ent->getCollider().m_position = ent->getPosition();
            }
    }

fe::sceneGraph::EntityHandle fe::sceneGraph::addEntity(fe::baseEntity *ent)
    {
        m_entities.push_back(ent);
        m_handles.push_back(m_entities.size() - 1);
        return m_handles.size() - 1;
    }

void fe::sceneGraph::removeEntity(fe::sceneGraph::EntityHandle handle)
    {
        if (m_handles.begin() + handle < m_handles.end())
            {
                m_entities.erase(m_entities.begin() + m_handles[handle]); 

                // since we are erasing an entity, all handles above it will become invalid. To prevent this, we will subtract all handles
                // above and including the current one by one.
                for (auto it = m_handles.begin() + handle; it != m_handles.end(); ++it) { (*it) -= 1; }
            }
        else
            {
                FE_LOG_WARNING("Cannot remove entity with handle \"", handle, "\"");
            }
    }

fe::baseEntity *fe::sceneGraph::getEntity(fe::sceneGraph::EntityHandle handle) const
    {
        if (handle >= m_handles.size())
            {
                FE_LOG_WARNING("Cannot retrieve entity with handle \"", handle, "\"");
                return nullptr;
            }
        return m_entities[m_handles[handle]];
    }

void fe::sceneGraph::draw(sf::RenderTarget &app)
    {
        m_batch.draw(app);
    }
