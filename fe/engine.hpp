// engineInit.hpp
// a class with a static function to init/deinit the engine
#pragma once
#define FLAT_ENGINE_EXPORT
#include "flatEngineExport.hpp"
#include "subsystems/memory/memoryManager.hpp"
#include "subsystems/memory/feNew.hpp"
#include "debug/logger.hpp"
#include "subsystems/input/inputManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "math/Vector2.hpp"

namespace fe
    {
        class engine
            {
                private:
                    fe::memoryManager m_memoryManager;
                    fe::logger *m_logger;
                    fe::inputManager *m_inputManager;

                    sf::RectangleShape *test;

                    sf::RenderWindow *m_renderWindow;

                    fe::Vector2d velocity;

                private:
                    float m_deltaTime;
                    float m_accumulator;
                                        
                private:
                    FLAT_ENGINE_API void handleEvents();
                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void draw();

                public:
                    FLAT_ENGINE_API engine(const float updateRate = 1.f / 60.f);

                    FLAT_ENGINE_API void loadWindow();

                    FLAT_ENGINE_API void startUp(unsigned long long totalMemory = 256_MiB, unsigned long long stackMemory = (256_MiB / 4));
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void run();
            };
    }