// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "spriteBatch.hpp"
#include "renderObject.hpp"
#include "../../threading/threadJob.hpp"
#include "../../../dataStructures/tree.hpp"
#include "../../resourceManager/fontData.hpp"
#include <type_traits>

namespace sf
    {
        class RenderTarget;
        class Font;
    }

namespace fe
    {
        class sceneGraph
            {
                private:
                    fe::sceneGraphObject m_baseNode;
                    fe::tree m_sceneRenderTree;

                    fe::poolAllocater<renderObject> m_renderObjects;
                    fe::poolAllocater<renderText> m_renderTextObjects;
                    fe::spriteBatch m_batch;

                    const unsigned int m_maxObjectsUntilThread;

                    FLAT_ENGINE_API void transformGraph(int nodeHandle);
                    FLAT_ENGINE_API void drawGraph(int nodeHandle, unsigned int &index);

                    FLAT_ENGINE_API void createRenderTextObject(sceneGraphObject *obj, const fe::fontData &font = fe::fontData());
                    FLAT_ENGINE_API int deleteRenderObject(renderObject *obj); // returns the parent node of the render object
                    FLAT_ENGINE_API int deleteRenderTextObject(renderText *obj); // returns the parent node of the render object

                public:
                    FLAT_ENGINE_API sceneGraph();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &window);
                    FLAT_ENGINE_API sceneGraphObject *allocateRenderObject();
                    FLAT_ENGINE_API sceneGraphObject *allocateRenderText();
                    template<typename ...Args>
                    void createSceneGraphObject(sceneGraphObject *obj, fe::lightVector2<unsigned int> texturePos = fe::lightVector2<unsigned int>(), int connected = -1, Args &&...args);
                    FLAT_ENGINE_API int deleteSceneObject(sceneGraphObject *obj); // returns the parent node of the render object

                    FLAT_ENGINE_API void connect(int a, int b); // connects object with node A to node B
                    FLAT_ENGINE_API void disconnect(int node); // Disconnects the node and attaches to the base scene node

                    FLAT_ENGINE_API fe::transformable &getGlobalTransform();

            };

        template<typename ...Args>
        void fe::sceneGraph::createSceneGraphObject(sceneGraphObject *obj, fe::lightVector2<unsigned int> texturePos, int connected, Args &&...args)
            {
                obj->m_texCoords[0] = texturePos.x;
                obj->m_texCoords[1] = texturePos.y;
                obj->m_graphNode = m_sceneRenderTree.addNode();
                connect(obj->m_graphNode, connected >= 0 ? connected : m_baseNode.m_graphNode);
                m_sceneRenderTree.getNode(obj->m_graphNode)->m_userData = obj;

                if ((fe::uInt8*)obj >= m_renderTextObjects.getBuffer() && (fe::uInt8*)obj <= m_renderTextObjects.getBuffer() + m_renderTextObjects.byteSize())
                    {
                        createRenderTextObject(obj, std::forward<Args>(args)...);
                    }
            }
    }