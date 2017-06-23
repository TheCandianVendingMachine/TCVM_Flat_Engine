#include "sceneGraph.hpp"
#include "../../../engine.hpp"
#include "../../resourceManager/resourceManager.hpp"
#include "../../physics/transformable.hpp"

void fe::sceneGraph::startUp()
    {
        m_renderObjects.startUp(FE_MAX_GAME_OBJECTS);
    }

void fe::sceneGraph::shutDown()
    {
        m_renderObjects.clear();
    }

void fe::sceneGraph::clear()
    {
        m_renderObjects.clear();
    }

void fe::sceneGraph::draw(sf::RenderTarget &window)
    {
        m_batch.clear();
        for (unsigned int i = 0; i < m_renderObjects.getObjectAllocCount(); i++)
            {
                renderObject *render = m_renderObjects.at(i);
                if (render && render->m_draw)
                    {
                        m_batch.add(render, fe::transformable());
                    }
            }

        sf::RenderStates states;
        states.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();

        m_batch.draw(window, states);
    }

fe::renderObject *fe::sceneGraph::createRenderObject()
    {
        return m_renderObjects.alloc();
    }

void fe::sceneGraph::deleteRenderObject(renderObject * obj)
    {
        return m_renderObjects.free(obj);
    }
