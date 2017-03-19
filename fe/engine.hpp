// engineInit.hpp
// a class with a static function to init/deinit the engine
#pragma once
#define FLAT_ENGINE_EXPORT
#include "flatEngineExport.hpp"
#include "subsystems/memory/memoryManager.hpp"
#include "subsystems/memory/feNew.hpp"
#include "subsystems/input/inputManager.hpp"
#include "subsystems/graphic/renderer.hpp"
#include "subsystems/graphic/sceneGraph.hpp"
#include "subsystems/gameState/gameStateMachine.hpp"
#include "debug/logger.hpp"

namespace fe
    {
        class engine
            {
                private:
                    fe::memoryManager m_memoryManager;
                    fe::renderer m_renderer;

                    fe::logger *m_logger;
                    fe::inputManager *m_inputManager;
                    fe::gameStateMachine *m_gameStateMachine;

                private:
                    static float m_fps; // used to calculate fps

                    const float m_deltaTime;
                    static float m_deltaTimeStatic;
                    float m_accumulator;
                                        
                private:
                    FLAT_ENGINE_API void handleEvents();
                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void draw();

                public:
                    FLAT_ENGINE_API engine(const float updateRate = 1.f / 60.f);

                    FLAT_ENGINE_API void startUp(unsigned long long totalMemory = 256_MiB,
                                                 unsigned long long stackMemory = 256_MiB * (9.f / 10.f));
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void run();

					FLAT_ENGINE_API renderer *getRenderer();
					
                    FLAT_ENGINE_API const static float getDeltaTime();
                    FLAT_ENGINE_API const static float getFPS();

                    FLAT_ENGINE_API void queueState(baseGameState *state);
					FLAT_ENGINE_API void queuePop();
            };
    }