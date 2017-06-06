#include "sceneGraph.hpp"
#include "../../entity/transformable.hpp"
#include "../../entity/baseEntity.hpp"
#include "../../entity/drawable.hpp"

#include "../../debug/logger.hpp"

#include "../../engine.hpp"
#include "../resourceManager/resourceManager.hpp"

#include <algorithm>
#include <fstream>

void fe::sceneGraph::onAdd(fe::baseEntity *object, fe::Handle objectHandle)
    {
        object->setHandle(objectHandle);
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

void fe::sceneGraph::cullObjects(fe::baseGameState &state)
    {
        auto &objects = getObjects();
        for (auto it = objects.begin(); it != objects.end();)
            {
                if ((*it)->isDestroyed())
                    {
                        (*it)->onDestroy(state);
                        m_animator.unsubscribe(getObject((*it)->getHandle()));
                        it = removeHandle((*it)->getHandle());
                    }
                else
                    {
                        it++;
                    }
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
        renderStates.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();

        m_tileMap.draw(app, renderStates);

        m_animator.updateTextures();
        m_batch.draw(app, renderStates);
    }

fe::tileMap &fe::sceneGraph::getTileMap()
    {
        return m_tileMap;
    }

fe::Handle fe::sceneGraph::addTexture(const char *filepath, const char *id)
    {
        auto texture = fe::engine::get().getResourceManager<sf::Texture>()->load(filepath, id);
        return addTexture(texture, fe::engine::get().getResourceManager<sf::Texture>()->getTextureOffset(id));
    }

fe::Handle fe::sceneGraph::addAnimation(const char *filepath, const char *id, fe::Vector2<unsigned int> frameSize)
    {
        auto texture = fe::engine::get().getResourceManager<sf::Texture>()->load(filepath, id);
        return addAnimation(texture, fe::engine::get().getResourceManager<sf::Texture>()->getTextureOffset(id), frameSize);
    }

fe::Handle fe::sceneGraph::addTexture(const sf::Texture *texture, fe::Vector2<unsigned int> offset)
    {
        return addAnimation(texture, offset, texture->getSize());
    }

fe::Handle fe::sceneGraph::addAnimation(const sf::Texture *texture, fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> frameSize)
    {
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

void fe::sceneGraph::unsubscribe(fe::animationActor *actor)
    {
        m_animator.unsubscribe(actor);
    }

void fe::sceneGraph::save(const char *filepath)
    {
        m_tileMap.serialize(m_serializer);

        std::ofstream out(filepath, std::ios::trunc);
        m_serializer.outData(out);
        out.close();

        m_serializer.clearData();
    }

void fe::sceneGraph::load(const char *filepath)
    {
        m_serializer.clearData();

        std::ifstream in(filepath);
        m_serializer.readData(in);
        in.close();

        m_tileMap.deserialize(m_serializer);
    }

fe::sceneGraph::~sceneGraph()
    {
        for (auto &ent : getObjects())
            {
                delete ent;
            }
    }
