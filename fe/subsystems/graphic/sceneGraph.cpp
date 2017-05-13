#include "sceneGraph.hpp"
#include "../../entity/transformable.hpp"
#include "../../entity/baseEntity.hpp"
#include "../../entity/drawable.hpp"

#include "../../debug/logger.hpp"

#include <algorithm>

void fe::sceneGraph::postInit()
    {
        m_textureBatch.createTexture();
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

        m_animator.updateTextures();
        m_batch.draw(app, renderStates);
    }

fe::Handle fe::sceneGraph::addTexture(sf::Texture *texture)
    {
        return addAnimation(texture, texture->getSize());
    }

fe::Handle fe::sceneGraph::addAnimation(sf::Texture *texture, fe::Vector2<unsigned int> frameSize)
    {
        fe::Vector2<unsigned int> offset = m_textureBatch.addTexture(*texture);
        return m_animator.addAnimation(frameSize, texture->getSize(), offset);
    }

void fe::sceneGraph::subscribe(fe::animationActor *actor, fe::Handle animation)
    {
        m_animator.subscribe(actor, animation);
    }

void fe::sceneGraph::unsubscribe(fe::animationActor *actor, fe::Handle animation)
    {
        m_animator.unsubscribe(actor, animation);
    }

fe::sceneGraph::~sceneGraph()
    {
        for (auto &ent : getObjects())
            {
                delete ent;
            }
    }
