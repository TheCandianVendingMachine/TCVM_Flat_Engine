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

namespace fe
    {
        class RenderTarget;
    }

namespace fe
    {
        class sceneGraph
            {
                private:
                    fe::poolAllocater<renderObject> m_renderObjects;
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

                    FLAT_ENGINE_API void drawParallel(unsigned int initialIndex, unsigned int endIndex);

                public:
                    FLAT_ENGINE_API sceneGraph();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void draw(sf::RenderTarget &window);
                    FLAT_ENGINE_API renderObject *createRenderObject();
                    FLAT_ENGINE_API void deleteRenderObject(renderObject *obj);

            };
    }