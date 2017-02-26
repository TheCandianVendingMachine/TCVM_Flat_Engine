// memoryManager.hpp
// a sub-system that allocates a block of memory on startup to be used around the program
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineInclude.hpp"
#include "../systemModule.hpp"

namespace fe 
    {
        class FLAT_ENGINE_API memoryManager : public systemModule
            {
                private:
                    char *m_allocatedMemory;

                    static memoryManager *m_instance;

                public:
                    void startUp();
                    void shutDown();

                    static memoryManager &get();
            };
    }