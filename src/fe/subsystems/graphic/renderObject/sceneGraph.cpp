#include "fe/subsystems/graphic/renderObject/sceneGraph.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/resourceManager/resourceManager.hpp"
#include "fe/subsystems/physics/transformable.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/subsystems/threading/threadPool.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::sceneGraph::createRenderTextObject(sceneGraphObject *obj, const fe::fontData &font)
    {
        static_cast<fe::renderText*>(obj)->m_fontData = font;
    }

int fe::sceneGraph::deleteRenderObject(renderObject *obj)
    {
        int parentNode = m_sceneRenderTree.getNode(obj->m_graphNode)->m_parent;
        m_renderObjects.free(obj);
        return parentNode;
    }

int fe::sceneGraph::deleteRenderTextObject(renderText *obj)
    {
        int parentNode = m_sceneRenderTree.getNode(obj->m_graphNode)->m_parent;
        m_renderTextObjects.free(obj);
        return parentNode;
    }

void fe::sceneGraph::addZ(int z)
    {
        int newNode = m_sceneRenderTree.addNode();
        connect(newNode, m_baseNode.m_graphNode);
        /* set user data 1 */
        m_sceneRenderTree.getNode(newNode)->m_userData = m_sceneGraphObjects.alloc();
        m_zOrderMap[z] = newNode;
    }

int fe::sceneGraph::getZ(int z)
    {
        return m_zOrderMap[z];
    }

fe::sceneGraph::sceneGraph() :
    m_maxObjectsUntilThread(2500) // 2500 = ~when threading and not threading reaches an intersection in FPS
    {
    }

void fe::sceneGraph::startUp()
    {
        m_renderObjects.startUp(FE_MAX_GAME_OBJECTS);
        m_renderTextObjects.startUp(FE_MAX_TEXT_OBJECTS);
        m_sceneGraphObjects.startUp(FE_MAX_Z_ORDER);
        m_baseNode.m_graphNode = m_sceneRenderTree.addNode();
        /* set user data 2 */
        m_sceneRenderTree.getNode(m_baseNode.m_graphNode)->m_userData = &m_baseNode;

        addZ(0);
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
        FE_ENGINE_PROFILE("scene_graph", "batch_clear");
        m_batch.clear();
        FE_END_PROFILE;
    }

void fe::sceneGraph::draw(sf::RenderTarget &window)
    {
        FE_ENGINE_PROFILE("scene_graph", "graph_transform");
        int nodeStack[FE_MAX_GAME_OBJECTS + FE_MAX_TEXT_OBJECTS + FE_MAX_Z_ORDER];
        unsigned int stackTop = 0;
        nodeStack[stackTop++] = m_baseNode.m_graphNode;

        while (stackTop > 0)
            {
                fe::priv::node *node = m_sceneRenderTree.getNode(nodeStack[stackTop - 1]);
                stackTop--;

                if (static_cast<fe::sceneGraphObject*>(node->m_userData)->m_draw)
                    {
                        fe::transformable *nodeTransform = &static_cast<fe::sceneGraphObject*>(node->m_userData)->m_transform;

                        for (auto &child : node->m_children)
                            {
                                const fe::transformable &realTransform = static_cast<fe::sceneGraphObject*>(m_sceneRenderTree.getNode(child)->m_userData)->m_transform;
                                fe::transformable *tempTransform = &static_cast<fe::sceneGraphObject*>(m_sceneRenderTree.getNode(child)->m_userData)->m_tempTransform;
                                *tempTransform = realTransform;
                                tempTransform->combine(*nodeTransform);
                                nodeTransform->setUpdateChildren(false);

                                nodeStack[stackTop++] = child;
                            }
                    }
            }
        FE_END_PROFILE;
        
        sf::RenderStates states;
        states.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();

        unsigned int index = 0;
        FE_ENGINE_PROFILE("scene_graph", "graph_draw");
        if (m_renderObjects.getObjectAllocCount() <= m_maxObjectsUntilThread || true) 
            {
                int nodeStack[FE_MAX_GAME_OBJECTS + FE_MAX_TEXT_OBJECTS + FE_MAX_Z_ORDER];
                unsigned int stackTop = 0;
                nodeStack[stackTop++] = m_baseNode.m_graphNode;

                while (stackTop > 0)
                    {
                        fe::priv::node *node = nullptr;
                        FE_ENGINE_PROFILE("scene_graph", "get_node");
                        node = m_sceneRenderTree.getNode(nodeStack[stackTop - 1]);
                        FE_END_PROFILE;
                        stackTop--;

                        fe::sceneGraphObject *object = static_cast<fe::sceneGraphObject*>(node->m_userData);
                        if (object->m_draw)
                            {
                                switch (object->m_type)
                                    {
                                        case OBJECT:
                                            FE_ENGINE_PROFILE("scene_graph", "addObjectToBatch");
                                            m_batch.add(static_cast<fe::renderObject*>(object), index);
                                            FE_END_PROFILE;
                                            break;
                                        case TEXT:
                                            FE_ENGINE_PROFILE("scene_graph", "addTextToBatch");
                                            m_batch.add(static_cast<fe::renderText*>(object), index);
                                            FE_END_PROFILE;
                                            break;
                                        default:
                                            break;
                                    }

                                FE_ENGINE_PROFILE("scene_graph", "iterate_children");
                                for (auto &childIndex : node->m_children)
                                    {
                                        nodeStack[stackTop++] = childIndex;
                                    }
                                FE_END_PROFILE;
                            }
                    }
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("scene_graph", "window_draw");
        m_batch.draw(window, states, index);
        FE_END_PROFILE;
    }

fe::sceneGraphObject *fe::sceneGraph::allocateRenderObject()
    {
        return m_renderObjects.alloc();
    }

fe::sceneGraphObject *fe::sceneGraph::allocateRenderText()
    {
        return m_renderTextObjects.alloc();
    }

void fe::sceneGraph::createSceneGraphObject(sceneGraphObject *obj, int connected, const fe::fontData &font)
    {
        obj->m_graphNode = m_sceneRenderTree.addNode();
        /* set user data 3 */
        m_sceneRenderTree.getNode(obj->m_graphNode)->m_userData = obj;
        if (connected < 0)
            {
                setZOrder(obj, obj->m_zPosition);
            }
        else 
            {
                connect(obj->m_graphNode, connected);
            }

        if ((fe::uInt8*)obj >= m_renderTextObjects.getBuffer() && (fe::uInt8*)obj <= m_renderTextObjects.getBuffer() + m_renderTextObjects.byteSize())
            {
                createRenderTextObject(obj, font);
            }
    }

int fe::sceneGraph::deleteSceneObject(sceneGraphObject *obj)
    {
        if ((fe::uInt8*)obj >= m_renderObjects.getBuffer() && (fe::uInt8*)obj <= m_renderObjects.getBuffer() + m_renderObjects.byteSize())
            {
                return deleteRenderObject(static_cast<fe::renderObject*>(obj));
            }
        else if ((fe::uInt8*)obj >= m_renderTextObjects.getBuffer() && (fe::uInt8*)obj <= m_renderTextObjects.getBuffer() + m_renderTextObjects.byteSize())
            {
                return deleteRenderTextObject(static_cast<fe::renderText*>(obj));
            }
        return -1;
    }

void fe::sceneGraph::setZOrder(sceneGraphObject *object, int z)
    {
        object->m_zPosition = z;
        setZOrder(object->m_graphNode, z);
    }

void fe::sceneGraph::connect(sceneGraphObject *objectA, sceneGraphObject *objectB)
    {
        connect(objectA->m_graphNode, objectB->m_graphNode);
    }

void fe::sceneGraph::disconnect(sceneGraphObject *object)
    {
        disconnect(object->m_graphNode);
    }

int fe::sceneGraph::getConnected(sceneGraphObject *object)
    {
        return getConnected(object->m_graphNode);
    }

void fe::sceneGraph::setZOrder(int node, int z)
    {
        if (!m_sceneRenderTree.nodeExists(m_baseNode.m_graphNode, getZ(z)))
            {
                addZ(z);
            }
        disconnect(node);
        if (!m_sceneRenderTree.nodeExists(getZ(z), node)) 
            {
                connect(node, getZ(z));
            }
        m_sceneRenderTree.sort(m_baseNode.m_graphNode, [this](int a, int b) {
            return getZ(a) < getZ(b);
        });
    }

void fe::sceneGraph::connect(int a, int b)
    {
        auto node = m_sceneRenderTree.getNode(a);
        m_sceneRenderTree.removeChild(node->m_parent, a);
        m_sceneRenderTree.addChild(b, a);
    }

void fe::sceneGraph::disconnect(int node)
    {
        connect(node, m_baseNode.m_graphNode);
    }

int fe::sceneGraph::getConnected(int node)
    {
        return m_sceneRenderTree.getNode(node)->m_parent;
    }

fe::transformable &fe::sceneGraph::getGlobalTransform()
    {
        return m_baseNode.m_transform;
    }
