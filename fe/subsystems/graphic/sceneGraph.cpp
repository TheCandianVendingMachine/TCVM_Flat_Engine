#include "sceneGraph.hpp"
#include "../../entity/transformable.hpp"
#include "../../entity/baseEntity.hpp"
#include "../../entity/drawable.hpp"

#include "../../debug/logger.hpp"

#include <algorithm>

void fe::sceneGraph::onAdd(fe::baseEntity *entity)
    {
        if (entity->m_texture)
            {
                auto texture = entity->m_texture;
                auto pos = m_textureBatch.addTexture(texture);

                entity->m_verticies[0].texCoords = fe::Vector2d(pos.x, pos.y).convertToSfVec2();
                entity->m_verticies[1].texCoords = fe::Vector2d(pos.x + texture->getSize().x, pos.y).convertToSfVec2();
                entity->m_verticies[2].texCoords = fe::Vector2d(pos.x + texture->getSize().x, pos.y + texture->getSize().y).convertToSfVec2();
                entity->m_verticies[3].texCoords = fe::Vector2d(pos.x, pos.y + texture->getSize().y).convertToSfVec2();

                entity->m_verticies[0].position = fe::Vector2d(0, 0).convertToSfVec2();
                entity->m_verticies[1].position = fe::Vector2d(texture->getSize().x, 0).convertToSfVec2();
                entity->m_verticies[2].position = fe::Vector2d(texture->getSize().x, texture->getSize().y).convertToSfVec2();
                entity->m_verticies[3].position = fe::Vector2d(0, texture->getSize().y).convertToSfVec2();
            }
    }

void fe::sceneGraph::postInit()
    {
        for (auto &entity : getObjects())
            {
                onAdd(entity);
            }
    }

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
        sf::RenderStates renderStates;
        renderStates.texture = &m_textureBatch.getTexture();

        m_batch.draw(app, renderStates);
    }

fe::sceneGraph::~sceneGraph()
    {
        for (auto &ent : getObjects())
            {
                delete ent;
            }
    }
