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
                    fe::logger m_logger;
                    fe::memoryManager m_memoryManager;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
            };
    }