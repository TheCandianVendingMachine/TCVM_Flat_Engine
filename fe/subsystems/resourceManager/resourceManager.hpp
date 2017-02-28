// resourceManager.hpp
// will load objects from file, copy them to memory, and then
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/poolAllocater.hpp"
#include "resourceObject.hpp"

#include <unordered_map>
#include <string>

namespace fe
    {
        class resourceManager
            {
                private:
                    std::unordered_map<std::string, resourceObject*> m_resources;
                    fe::poolAllocater<resourceObject> m_resourceAllocater;

                public:
                    FLAT_ENGINE_API void startUp(size_t amountOfResources);

                    FLAT_ENGINE_API void add(const std::string &id, const std::string &filepath, fe::resourceObjectType::resourceObjectType type);
                    FLAT_ENGINE_API void remove(const std::string &id);
                    FLAT_ENGINE_API resourceObject *get(const std::string &id);

                    FLAT_ENGINE_API void clear();

            };
    }