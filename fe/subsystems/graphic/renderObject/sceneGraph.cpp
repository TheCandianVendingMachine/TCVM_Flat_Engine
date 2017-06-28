#include "sceneGraph.hpp"
#include "../../../engine.hpp"
#include "../../resourceManager/resourceManager.hpp"
#include "../../physics/transformable.hpp"
#include "../../../debug/profiler.hpp"
#include "../../threading/threadPool.hpp"

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

void fe::sceneGraph::preDraw()
    {
        FE_PROFILE("scene_graph_batch_draw");
        m_batch.clear();

        fe::engine::get().getThreadPool().addJob(m_jobA);
        fe::engine::get().getThreadPool().addJob(m_jobB);
        fe::engine::get().getThreadPool().addJob(m_jobC);
        fe::engine::get().getThreadPool().addJob(m_jobD);

        FE_END_PROFILE;
    }

void fe::sceneGraph::draw(sf::RenderTarget &window)
    {
        sf::RenderStates states;
        states.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();

        FE_PROFILE("scene_graph_wait_draw");
        fe::engine::get().getThreadPool().waitFor(m_jobA);
        fe::engine::get().getThreadPool().waitFor(m_jobB);
        fe::engine::get().getThreadPool().waitFor(m_jobC);
        fe::engine::get().getThreadPool().waitFor(m_jobD);
        FE_END_PROFILE;

        FE_PROFILE("scene_graph_window_draw");
        m_batch.draw(window, states);
        FE_END_PROFILE;
    }

fe::renderObject *fe::sceneGraph::createRenderObject()
    {
        fe::renderObject *allocated = m_renderObjects.alloc();
        unsigned int allocCount = m_renderObjects.getObjectAllocCount();
        if (allocCount < 4)
            {
                if (allocCount < 4)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = true;
                        m_jobD.m_active = false;
                    }
                if (allocCount < 3)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = false;

                    }
                if (allocCount < 2)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = false;
                    }
                if (allocCount < 1)
                    {
                        m_jobA.m_active = false;
                    }

                unsigned int objectCountQuart = allocCount;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
        else
            {
                m_jobA.m_active = true;
                m_jobB.m_active = true;
                m_jobC.m_active = true;
                m_jobD.m_active = true;

                unsigned int objectCountQuart = allocCount / 4;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
        return allocated;
    }

void fe::sceneGraph::deleteRenderObject(renderObject *obj)
    {
        m_renderObjects.free(obj);
        unsigned int allocCount = m_renderObjects.getObjectAllocCount();
        if (allocCount < 4)
            {
                if (allocCount < 4)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = true;
                        m_jobD.m_active = false;
                    }
                if (allocCount < 3)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = false;

                    }
                if (allocCount < 2)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = false;
                    }
                if (allocCount < 1)
                    {
                        m_jobA.m_active = false;
                    }

                unsigned int objectCountQuart = allocCount;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
        else
            {
                m_jobA.m_active = true;
                m_jobB.m_active = true;
                m_jobC.m_active = true;
                m_jobD.m_active = true;

                unsigned int objectCountQuart = allocCount / 4;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
    }

fe::sceneGraph::renderJob::renderJob(fe::poolAllocater<renderObject> &renderObjects, fe::spriteBatch &batch) : m_renderObjects(renderObjects), m_batch(batch)
    {}

bool fe::sceneGraph::renderJob::execute()
    {
        fe::transformable a;
        unsigned int index = m_initialIndex;
        for (unsigned int i = m_initialIndex; i < m_endIndex; i++)
            {
                renderObject *render = m_renderObjects.at(i);
                // taking advantage of branch prediction. The CPU is assuming the statement will be false
                // so I will let it be false. Because most objects in the indicies will exist, this should
                // increase performance
                if (!render || !render->m_draw)
                    {}
                else
                    {
                        m_batch.add(render, a, index, m_initialIndex);
                    }
            }
        return true;
    }
