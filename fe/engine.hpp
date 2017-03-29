// engineInit.hpp
// a class with a static function to init/deinit the engine
#pragma once
#define FLAT_ENGINE_EXPORT
#include "flatEngineExport.hpp"
#include "subsystems/memory/memoryManager.hpp"
#include "subsystems/memory/feNew.hpp"
#include "subsystems/graphic/renderer.hpp"
#include "time/clock.hpp"

namespace fe
    {
        class logger;
        class inputManager;
        class gameStateMachine;
        class eventSender;
        class baseGameState;

        class engine
            {
                private:
                    fe::memoryManager m_memoryManager;
                    fe::renderer m_renderer;

                    fe::logger *m_logger;
                    fe::inputManager *m_inputManager;
                    fe::gameStateMachine *m_gameStateMachine;
                    fe::eventSender *m_eventSender;

                private:
                    fe::clock m_fpsClock;
                    int m_elapsedFrames;

                    const float m_deltaTime;
                    float m_accumulator;
                    float m_fps; // used to calculate fps

                    fe::Vector2d m_screenSize;
                    fe::Vector2d m_mousePosition;

                    static fe::engine *m_instance;
                                        
                private:
                    FLAT_ENGINE_API void handleEvents();
                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void draw();
                    FLAT_ENGINE_API void calcFPS();

                public:
                    FLAT_ENGINE_API engine(const float updateRate = 1.f / 60.f);

                    FLAT_ENGINE_API void startUp(unsigned long long totalMemory = 256_MiB,
                                                 unsigned long long stackMemory = 256_MiB * (9.f / 10.f));
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API static const engine &get();

                    FLAT_ENGINE_API void run();

                    FLAT_ENGINE_API const float getDeltaTime() const;
                    FLAT_ENGINE_API const float getFPS() const;
                    FLAT_ENGINE_API const fe::Vector2d getWindowSize() const;

                    FLAT_ENGINE_API const fe::Vector2d getMousePos() const;

                    FLAT_ENGINE_API void queueState(fe::baseGameState *state);
					FLAT_ENGINE_API void queuePop();


                    FLAT_ENGINE_API fe::eventSender *getEventSender() const;
            };
    }