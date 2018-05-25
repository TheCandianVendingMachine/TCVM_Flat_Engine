#include "fe/subsystems/graphic/renderGraph.hpp"
#include "fe/debug/profiler.hpp"

void fe::renderGraph::addZ(int z)
    {
        int newNode = m_sceneRenderTree.addNode();
        connect(newNode, m_baseNode);
        /* set user data 1 */
        m_sceneRenderTree.getNode(newNode)->m_userData = createZOrderProxy();
        m_zOrderMap[z] = newNode;
    }

int fe::renderGraph::getZ(int z)
    {
        return m_zOrderMap[z];
    }

fe::renderGraph::renderGraph(unsigned int maxObjectsUntilThread) :
    m_maxObjectsUntilThread(maxObjectsUntilThread)
    {
    }

void fe::renderGraph::startUp()
    {
        m_baseNode = m_sceneRenderTree.addNode();
        addZ(0);
        onStartUp();
    }

void fe::renderGraph::shutDown()
    {
        onShutDown();
    }

void fe::renderGraph::clear()
    {
        onClear();
    }

int fe::renderGraph::addObjectToGraph(void *object, int connected, int zPos)
    {
        unsigned int graphNode = m_sceneRenderTree.addNode();
        m_sceneRenderTree.getNode(graphNode)->m_userData = object;

        if (connected < 0)
            {
                setZOrder(graphNode, zPos);
            }
        else 
            {
                connect(graphNode, connected);
            }

        onObjectAdd(object, graphNode);

        return graphNode;
    }

int fe::renderGraph::removeObjectFromGraph(void *object)
    {
        return removeNodeFromGraph(getNodeFromObject(object));
    }

int fe::renderGraph::removeNodeFromGraph(int node)
    {
        onObjectRemove(m_sceneRenderTree.getNode(node)->m_userData, node);
        return m_sceneRenderTree.getNode(node)->m_parent;
    }

void fe::renderGraph::draw(sf::RenderTarget &window)
    {
        int nodeStack[FE_MAX_GAME_OBJECTS + FE_MAX_TEXT_OBJECTS + FE_MAX_Z_ORDER];
        unsigned int stackTop = 0;
        nodeStack[stackTop++] = m_baseNode;
        FE_ENGINE_PROFILE("render_graph", "graph_transform");
        // Transform entire graph
        while (stackTop > 0)
            {
                fe::priv::node *node = m_sceneRenderTree.getNode(nodeStack[stackTop - 1]);
                stackTop--;

                if (!node->m_userData || renderNode(node))
                    {
                        fe::transformable *nodeTransform = &m_baseTransform;
                        if (node != m_sceneRenderTree.getNode(m_baseNode)) 
                            {
                                nodeTransform = getNodeTempTransform(node);
                            }

                        for (auto &child : node->m_children)
                            {
                                const fe::transformable *realTransform = getNodeTransform(m_sceneRenderTree.getNode(child));
                                fe::transformable *tempTransform = getNodeTempTransform(m_sceneRenderTree.getNode(child));
                                *tempTransform = *realTransform;
                                tempTransform->combine(*nodeTransform);

                                nodeStack[stackTop++] = child;
                            }
                    }
            }
        FE_END_PROFILE;
        
        sf::RenderStates states;
        states.texture = getRenderTexture();

        unsigned int index = 0;
        FE_PROFILE("render_graph", "graph_draw");
        std::memset(nodeStack, 0, FE_MAX_GAME_OBJECTS + FE_MAX_TEXT_OBJECTS + FE_MAX_Z_ORDER);
        stackTop = 0;
        nodeStack[stackTop++] = m_baseNode;

        while (stackTop > 0)
            {
                // Render entire graph
                fe::priv::node *node = nullptr;
                FE_ENGINE_PROFILE("render_graph", "get_node");
                node = m_sceneRenderTree.getNode(nodeStack[stackTop - 1]);
                FE_END_PROFILE;
                stackTop--;

                FE_ENGINE_PROFILE("render_graph", "render_graph");
                if (!node->m_userData || renderNode(node))
                    {
                        if (node->m_userData)
                            {
                                drawNode(node, window);
                            }
                        FE_ENGINE_PROFILE("render_graph", "iterate_children");
                        for (auto &childIndex : node->m_children)
                            {
                                nodeStack[stackTop++] = childIndex;
                            }
                        FE_END_PROFILE;
                    }
                FE_END_PROFILE;
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("render_graph", "window_draw");
        drawToScreen(window, states);
        FE_END_PROFILE;
    }

void fe::renderGraph::setZOrder(int node, int z)
    {
        if (!m_sceneRenderTree.nodeExists(m_baseNode, getZ(z)))
            {
                addZ(z);
            }
        disconnect(node);
        if (!m_sceneRenderTree.nodeExists(getZ(z), node)) 
            {
                connect(node, getZ(z));
            }
        m_sceneRenderTree.sort(m_baseNode);
    }

void fe::renderGraph::connect(int a, int b)
    {
        auto node = m_sceneRenderTree.getNode(a);
        m_sceneRenderTree.removeChild(node->m_parent, a);
        m_sceneRenderTree.addChild(b, a);
    }

void fe::renderGraph::disconnect(int node)
    {
        connect(node, m_baseNode);
    }

int fe::renderGraph::getConnected(int node)
    {
        return m_sceneRenderTree.getNode(node)->m_parent;
    }

fe::transformable &fe::renderGraph::getGlobalTransform()
    {
        return m_baseTransform;
    }
