#include "sceneGraph.hpp"
#include "../../../engine.hpp"
#include "../../resourceManager/resourceManager.hpp"

void fe::sceneGraph::startUp()
    {
        m_renderObjects.startUp(FE_MAX_GAME_OBJECTS);
    }

void fe::sceneGraph::shutDown()
    {
        m_renderObjects.clear();
    }

void fe::sceneGraph::draw(sf::RenderTarget &window)
    {
        m_batch.clear();
        for (unsigned int i = 0; i < FE_MAX_GAME_OBJECTS; i++)
            {
                renderObject *render = m_renderObjects.at(i);
                if (render && render->m_draw)
                    {
                        //m_batch.add(render, fe::transformable);
                    }
            }

        sf::RenderStates states;
        states.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();

        m_batch.draw(window, states);
    }
