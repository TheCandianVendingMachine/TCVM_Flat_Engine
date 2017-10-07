// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "spriteBatch.hpp"
#include "renderObject.hpp"
#include "renderText.hpp"
#include "../../threading/threadJob.hpp"
#include "../../ai/graph.hpp"

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
                    // not a generic graph, but it works for our purpose
                    fe::graph m_sceneRenderGraph;

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

                public:
                    FLAT_ENGINE_API sceneGraph();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &window);
                    FLAT_ENGINE_API renderObject *createRenderObject();
                    FLAT_ENGINE_API renderText *createRenderTextObject(sf::Font *font);
                    FLAT_ENGINE_API void deleteRenderObject(renderObject *obj);
                    FLAT_ENGINE_API void deleteRenderTextObject(renderText *obj);

            };
    }