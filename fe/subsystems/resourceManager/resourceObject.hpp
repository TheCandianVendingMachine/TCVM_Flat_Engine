// resourceObject.hpp
// a struct containing meta-data for any sort of resource that can be loaded
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

namespace fe
    {
        namespace resourceObjectType
            {
                enum resourceObjectType
                    {
                        RESOURCE_NULL       = 1 << 0,
                        RESOURCE_TEXTURE    = 1 << 1,
                    };
            }

        struct resourceObject
            {
                char *m_filepath;
                void *m_resourceData;

                virtual void load(const char *filepath) {}

            };

        struct imageObject : public resourceObject
            {
                FLAT_ENGINE_API void load(const char *filepath) override;
            };
    }