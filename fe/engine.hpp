// engineInit.hpp
// The game engine. The main game loop and all of the systems associated with it
#pragma once
#define FLAT_ENGINE_EXPORT
#include "flatEngineExport.hpp"
#include "subsystems/memory/memoryManager.hpp"
#include "subsystems/graphic/renderer.hpp"
#include "subsystems/graphic/camera.hpp"
#include "math/random.hpp"
#include "time/clock.hpp"
#include "typeDefines.hpp"

namespace sf
    {
        class Texture;
        class Font;
    }

namespace fe
    {
        class logger;
        class inputManager;
        class gameStateMachine;
        class eventSender;
        class baseGameState;
        class physicsEngine;
        class collisionWorld;
        template<unsigned int>
        class threadPool;
        class debugDraw;
        class profilerLogger;
        class localizationStorage;
        class guiPrefabricatedElements;
        class scriptManager;

        template<typename T>
        class resourceManager;

        class engine
            {
                private:
                    fe::memoryManager m_memoryManager;
                    fe::renderer m_renderer;
                    fe::random m_random;

                    fe::threadPool<8> *m_threadPool;
                    fe::eventSender *m_eventSender;
                    fe::gameStateMachine *m_gameStateMachine;
                    fe::logger *m_logger;
                    fe::profilerLogger *m_profileLogger;
                    fe::debugDraw *m_debugDraw;
                    fe::inputManager *m_inputManager;
                    fe::physicsEngine *m_physicsEngine;
                    fe::collisionWorld *m_collisionWorld;

                    fe::resourceManager<sf::Texture> *m_textureManager;
                    fe::resourceManager<sf::Font> *m_fontManager;

                    fe::localizationStorage *m_localization;
                    fe::guiPrefabricatedElements *m_prefabGuiElements;

                    fe::scriptManager *m_scriptManager;

                    fe::camera m_defaultCamera;

                private:
                    fe::clock m_fpsClock;
                    int m_elapsedFrames;

                    const float m_deltaTime;
                    float m_accumulator;
                    float m_elapsedTime;
                    float m_fps; // used to calculate fps

                    fe::Vector2d m_screenSize;
                    fe::Vector2d m_mousePosition;

                    bool m_shutDown;
                    static fe::engine *m_instance;
                                        
                private:
                    FLAT_ENGINE_API void handleEvents();
                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void draw();
                    FLAT_ENGINE_API void calcFPS();

                public:
                    FLAT_ENGINE_API engine(const float updateRate = 1.f / 60.f);

                    FLAT_ENGINE_API void startUp(fe::uInt64 totalMemory = 256_MiB,
                                                fe::uInt64 stackMemory = 256_MiB * (8.f / 10.f),
                                                fe::uInt64 dynamicMemory = 256_MiB * (1.f / 10.f));
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void close() const;

                    FLAT_ENGINE_API static const engine &get();

                    FLAT_ENGINE_API void run();

                    FLAT_ENGINE_API const float getDeltaTime() const;
                    FLAT_ENGINE_API const float getFPS() const;
                    FLAT_ENGINE_API fe::time getElapsedTime() const;

                    FLAT_ENGINE_API const fe::Vector2<unsigned int> getWindowSize() const;

                    FLAT_ENGINE_API const fe::Vector2d getMousePos() const;

                    FLAT_ENGINE_API fe::gameStateMachine &getStateMachine() const;
                    FLAT_ENGINE_API fe::baseGameState &getCurrentState() const;
                    FLAT_ENGINE_API fe::inputManager &getInputManager() const;
                    FLAT_ENGINE_API const fe::renderer &getRenderer() const;
                    FLAT_ENGINE_API fe::eventSender &getEventSender() const;
                    FLAT_ENGINE_API fe::physicsEngine &getPhysicsEngine() const;
                    FLAT_ENGINE_API fe::collisionWorld &getCollisionWorld() const;
                    FLAT_ENGINE_API fe::threadPool<8> &getThreadPool() const;
                    FLAT_ENGINE_API fe::localizationStorage &getLocalization() const;
                    FLAT_ENGINE_API fe::guiPrefabricatedElements &getPrefabGui() const;
                    FLAT_ENGINE_API const fe::camera &getDefaultCamera() const;

                    FLAT_ENGINE_API void loadResources(const char *resourcesFile);

                    template<typename T>
                    fe::resourceManager<T>              *getResourceManager() const;
                    template<>
                    fe::resourceManager<sf::Texture>    *getResourceManager() const;
                    template<>
                    fe::resourceManager<sf::Font>       *getResourceManager() const;

                    FLAT_ENGINE_API ~engine();
            };

        template<typename T>
        inline fe::resourceManager<T> *engine::getResourceManager() const
            {
                return nullptr;
            }

        template<>
        inline fe::resourceManager<sf::Texture> *engine::getResourceManager() const
            {
                return m_textureManager;
            }

        template<>
        inline fe::resourceManager<sf::Font> *engine::getResourceManager() const
            {
                return m_fontManager;
            }

    }