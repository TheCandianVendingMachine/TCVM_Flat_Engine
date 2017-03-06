// engineInit.hpp
// a class with a static function to init/deinit the engine
#pragma once
#define FLAT_ENGINE_EXPORT
#include "flatEngineExport.hpp"
#include "subsystems/memory/memoryManager.hpp"
#include "debug/logger.hpp"

namespace fe
    {
        class engine
            {
                private:
                    fe::memoryManager m_memoryManager;

                    fe::logger *m_logger;

                public:
                    FLAT_ENGINE_API engine();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
            };
    }