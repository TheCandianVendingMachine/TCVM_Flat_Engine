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

                    struct renderJob : public fe::threadJob
                        {
                            fe::poolAllocater<renderObject> &m_renderObjects;
                            fe::spriteBatch &m_batch;

                            unsigned int m_initialIndex;
                            unsigned int m_endIndex;

                            FLAT_ENGINE_API renderJob(fe::poolAllocater<renderObject> &renderObjects, fe::spriteBatch &batch);
                            FLAT_ENGINE_API bool execute();
                        };

                    renderJob m_jobA;
                    renderJob m_jobB;
                    renderJob m_jobC;
                    renderJob m_jobD;

                    const unsigned int m_maxObjectsUntilThread;

                    FLAT_ENGINE_API void transformGraph(int nodeHandle);
                    FLAT_ENGINE_API void drawGraph(int nodeHandle, unsigned int &index);

                    FLAT_ENGINE_API int deleteRenderObject(renderObject *obj); // returns the parent node of the render object
                    FLAT_ENGINE_API int deleteRenderTextObject(renderText *obj); // returns the parent node of the render object

                public:
                    FLAT_ENGINE_API sceneGraph();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &window);
                    FLAT_ENGINE_API renderObject *createRenderObject(int connected = -1);
                    FLAT_ENGINE_API renderText *createRenderTextObject(const sf::Font *font = nullptr, int connected = -1);
                    FLAT_ENGINE_API int deleteSceneObject(sceneGraphObject *obj); // returns the parent node of the render object

                    FLAT_ENGINE_API void connect(int a, int b); // connects object with node A to node B
                    FLAT_ENGINE_API void disconnect(int node); // Disconnects the node and attaches to the base scene node

                    FLAT_ENGINE_API fe::transformable &getGlobalTransform();

            };
    }