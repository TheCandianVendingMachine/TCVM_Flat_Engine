#include "fe/subsystems/graphic/renderGraph.hpp"
#include "fe/debug/profiler.hpp"

void fe::renderGraph::addZ(int z)
    {
        int newNode = m_sceneRenderTree.addNode();
        connect(newNode, m_baseNode);
        /* set user data 1 */
        m_sceneRenderTree.getNode(newNode)->m_userData = nullptr;
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

void fe::renderGraph::draw(sf::RenderTarget &window)
    {
        int nodeStack[FE_MAX_GAME_OBJECTS + FE_MAX_TEXT_OBJECTS + FE_MAX_Z_ORDER];
        unsigned int stackTop = 0;
        nodeStack[stackTop++] = m_baseNode;
        FE_ENGINE_PROFILE("render_graph", "graph_transform");
        while (stackTop > 0)
            {
                fe::priv::node *node = m_sceneRenderTree.getNode(nodeStack[stackTop - 1]);
                stackTop--;

                if (renderNode(node))
                    {
                        fe::transformable *nodeTransform = getNodeTransform(node);
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
                fe::priv::node *node = nullptr;
                FE_ENGINE_PROFILE("render_graph", "get_node");
                node = m_sceneRenderTree.getNode(nodeStack[stackTop - 1]);
                FE_END_PROFILE;
                stackTop--;

                if (renderNode(node))
                    {
                        drawNode(node);
                        FE_ENGINE_PROFILE("render_graph", "iterate_children");
                        for (auto &childIndex : node->m_children)
                            {
                                nodeStack[stackTop++] = childIndex;
                            }
                        FE_END_PROFILE;
                    }
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
