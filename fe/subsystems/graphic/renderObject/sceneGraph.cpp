#include "sceneGraph.hpp"
#include "../../../engine.hpp"
#include "../../resourceManager/resourceManager.hpp"
#include "../../physics/transformable.hpp"
#include "../../../debug/profiler.hpp"
#include "../../threading/threadPool.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::sceneGraph::transformGraph(int nodeHandle)
    {
        auto node = m_sceneRenderTree.getNode(nodeHandle);
        fe::transformable *nodeTransform = &static_cast<fe::sceneGraphObject*>(node->m_userData)->m_transform;

        for (auto &child : node->m_children)
            {
                fe::transformable &realTransform = static_cast<fe::sceneGraphObject*>(m_sceneRenderTree.getNode(child)->m_userData)->m_transform;
                fe::transformable *tempTransform = &static_cast<fe::sceneGraphObject*>(m_sceneRenderTree.getNode(child)->m_userData)->m_tempTransform;
                *tempTransform = realTransform;
                tempTransform->combine(*nodeTransform);
                nodeTransform->setUpdateChildren(false);
                transformGraph(child);
            }
    }

void fe::sceneGraph::drawGraph(int nodeHandle, unsigned int &index)
    {
        auto node = m_sceneRenderTree.getNode(nodeHandle);
        fe::sceneGraphObject *obj = static_cast<fe::sceneGraphObject*>(node->m_userData);

        if (obj->m_type == OBJECT)
            {
                m_batch.add(static_cast<fe::renderObject*>(obj), index);
            }
        else if (obj->m_type == TEXT)
            {
                m_batch.add(static_cast<fe::renderText*>(obj), index);
            }

        for (auto &child : node->m_children)
            {
                drawGraph(child, index);
            }
    }

int fe::sceneGraph::deleteRenderObject(renderObject *obj)
    {
        int parentNode = m_sceneRenderTree.getNode(obj->m_graphNode)->m_parent;
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
        return parentNode;
    }

int fe::sceneGraph::deleteRenderTextObject(renderText *obj)
    {
        int parentNode = m_sceneRenderTree.getNode(obj->m_graphNode)->m_parent;
        m_renderTextObjects.free(obj);
        return parentNode;
    }

fe::sceneGraph::sceneGraph() :
    m_jobA(m_renderObjects, m_batch),
    m_jobB(m_renderObjects, m_batch),
    m_jobC(m_renderObjects, m_batch),
    m_jobD(m_renderObjects, m_batch),
    m_maxObjectsUntilThread(2500) // 2500 = ~when threading and not threading reaches an intersection in FPS
    {
    }

void fe::sceneGraph::startUp()
    {
        m_renderObjects.startUp(FE_MAX_GAME_OBJECTS);
        m_renderTextObjects.startUp(FE_MAX_TEXT_OBJECTS);
        m_baseNode.m_graphNode = m_sceneRenderTree.addNode();
        m_sceneRenderTree.getNode(m_baseNode.m_graphNode)->m_userData = &m_baseNode;
    }

void fe::sceneGraph::shutDown()
    {
        m_renderObjects.clear();
        m_renderTextObjects.clear();
    }

void fe::sceneGraph::clear()
    {
        m_renderObjects.clear();
    }

void fe::sceneGraph::preDraw()
    {
        FE_ENGINE_PROFILE("scene_graph", "batch_draw");
        m_batch.clear();

        if (m_renderObjects.getObjectAllocCount() <= m_maxObjectsUntilThread) 
            {
            }
        else
            {
                fe::engine::get().getThreadPool().addJob(m_jobA);
                fe::engine::get().getThreadPool().addJob(m_jobB);
                fe::engine::get().getThreadPool().addJob(m_jobC);
                fe::engine::get().getThreadPool().addJob(m_jobD);
            }

        FE_END_PROFILE;
    }

void fe::sceneGraph::draw(sf::RenderTarget &window)
    {
        transformGraph(m_baseNode.m_graphNode);

        sf::RenderStates states;
        states.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();

        if (m_renderObjects.getObjectAllocCount() <= m_maxObjectsUntilThread) 
            {
                unsigned int index = 0;
                /*for (unsigned int i = 0; i < m_renderObjects.getObjectAllocCount(); i++)
                    {
                        renderObject *render = m_renderObjects.at(i);
                        // taking advantage of branch prediction. The CPU is assuming the statement will be false
                        // so I will let it be false. Because most objects in the indicies will exist, this should
                        // increase performance
                        if (!render || !render->m_draw)
                            {}
                        else
                            {
                                m_batch.add(render, index);
                            }
                    }*/
                drawGraph(m_baseNode.m_graphNode, index);
            }
        else
            {
                FE_ENGINE_PROFILE("scene_graph", "wait_draw");
                fe::engine::get().getThreadPool().waitFor(m_jobA);
                fe::engine::get().getThreadPool().waitFor(m_jobB);
                fe::engine::get().getThreadPool().waitFor(m_jobC);
                fe::engine::get().getThreadPool().waitFor(m_jobD);
                FE_END_PROFILE;
            }

        FE_ENGINE_PROFILE("scene_graph", "window_draw");
        m_batch.draw(window, states, m_renderObjects.getObjectAllocCount());
        FE_ENGINE_PROFILE("scene_graph", "text_draw");
        for (unsigned int i = 0; i < m_renderTextObjects.getObjectAllocCount(); i++)
            {
                /*fe::renderText *text = m_renderTextObjects.at(i);
                text->m_text.setColor(sf::Color(text->m_vertColour[0], text->m_vertColour[1], text->m_vertColour[2], text->m_vertColour[3]));
                text->m_text.setPosition(text->m_transform.getPosition().convertToSfVec2());
                window.draw(text->m_text);*/
            }
        FE_END_PROFILE;
        FE_END_PROFILE;
    }

fe::renderObject *fe::sceneGraph::createRenderObject(int connected)
    {
        fe::renderObject *allocated = m_renderObjects.alloc();
        allocated->m_graphNode = m_sceneRenderTree.addNode();
        connect(allocated->m_graphNode, connected >= 0 ? connected : m_baseNode.m_graphNode);
        m_sceneRenderTree.getNode(allocated->m_graphNode)->m_userData = allocated;

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

fe::renderText *fe::sceneGraph::createRenderTextObject(const sf::Font *font, int connected)
    {
        fe::renderText *text = m_renderTextObjects.alloc();
        text->m_graphNode = m_sceneRenderTree.addNode();
        connect(text->m_graphNode, connected >= 0 ? connected : m_baseNode.m_graphNode);
        m_sceneRenderTree.getNode(text->m_graphNode)->m_userData = text;
        text->m_font = font;
        return text;
    }

int fe::sceneGraph::deleteSceneObject(sceneGraphObject *obj)
    {
        if ((fe::uInt8*)obj >= m_renderObjects.getBuffer() && (fe::uInt8*)obj <=m_renderObjects.getBuffer() + m_renderObjects.byteSize())
            {
                return deleteRenderObject(static_cast<fe::renderObject*>(obj));
            }
        else if ((fe::uInt8*)obj >= m_renderTextObjects.getBuffer() && (fe::uInt8*)obj <= m_renderTextObjects.getBuffer() + m_renderTextObjects.byteSize())
            {
                return deleteRenderTextObject(static_cast<fe::renderText*>(obj));
            }
        return -1;
    }

void fe::sceneGraph::connect(int a, int b)
    {
        auto node = m_sceneRenderTree.getNode(a);
        m_sceneRenderTree.removeChild(a, node->m_parent);
        m_sceneRenderTree.addChild(b, a);
    }

void fe::sceneGraph::disconnect(int node)
    {
        connect(node, m_baseNode.m_graphNode);
    }

fe::transformable &fe::sceneGraph::getGlobalTransform()
    {
        return m_baseNode.m_transform;
    }

fe::sceneGraph::renderJob::renderJob(fe::poolAllocater<renderObject> &renderObjects, fe::spriteBatch &batch) : m_renderObjects(renderObjects), m_batch(batch)
    {}

bool fe::sceneGraph::renderJob::execute()
    {
        unsigned int index = m_initialIndex * 4;
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
                        m_batch.add(render, index);
                    }
            }
        return true;
    }
