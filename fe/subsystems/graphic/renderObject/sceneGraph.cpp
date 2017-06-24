#include "sceneGraph.hpp"
#include "../../../engine.hpp"
#include "../../resourceManager/resourceManager.hpp"
#include "../../physics/transformable.hpp"
#include "../../../debug/profiler.hpp"
#include <thread>

void fe::sceneGraph::drawParallel(unsigned int initialIndex, unsigned int endIndex)
    {
        
    }

fe::sceneGraph::sceneGraph() : 
    m_jobA(m_renderObjects, m_batch),
    m_jobB(m_renderObjects, m_batch),
    m_jobC(m_renderObjects, m_batch),
    m_jobD(m_renderObjects, m_batch)
    {
    }

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
        fe::transformable a;
        FE_PROFILE("scene_graph_batch_draw");
        m_batch.clear();

        unsigned int objectCountQuart = m_renderObjects.getObjectAllocCount() / 4;

        std::thread draw1(&fe::sceneGraph::drawParallel, this, 0 * objectCountQuart, objectCountQuart * 1);
        std::thread draw2(&fe::sceneGraph::drawParallel, this, 1 * objectCountQuart, objectCountQuart * 2);
        std::thread draw3(&fe::sceneGraph::drawParallel, this, 2 * objectCountQuart, objectCountQuart * 3);
        std::thread draw4(&fe::sceneGraph::drawParallel, this, 3 * objectCountQuart, objectCountQuart * 4);
        
        draw1.join();
        draw2.join();
        draw3.join();
        draw4.join();

        FE_END_PROFILE;

        sf::RenderStates states;
        states.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();

        FE_PROFILE("scene_graph_window_draw");
        m_batch.draw(window, states);
        FE_END_PROFILE;
    }

fe::renderObject *fe::sceneGraph::createRenderObject()
    {
        return m_renderObjects.alloc();
    }

void fe::sceneGraph::deleteRenderObject(renderObject *obj)
    {
        return m_renderObjects.free(obj);
    }

fe::sceneGraph::renderJob::renderJob(fe::poolAllocater<renderObject> &renderObjects, fe::spriteBatch &batch) : m_renderObjects(renderObjects), m_batch(batch)
    {}

bool fe::sceneGraph::renderJob::execute()
    {
        fe::transformable a;
        unsigned int index = 0;
        for (unsigned int i = m_initialIndex; i < m_endIndex; i++)
            {
                renderObject *render = m_renderObjects.at(i);
                if (render && render->m_draw)
                    {
                        m_batch.add(render, a, index, m_initialIndex);
                    }
            }
        return true;
    }
