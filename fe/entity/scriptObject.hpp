// scriptObject.hpp
// A basic object that the scripting system uses to gain knowledge about objects
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"

namespace fe
    {
        class baseEntity;
        class scriptObject
            {
                private:
                    baseEntity const *m_entity;

                public:
                    FLAT_ENGINE_API scriptObject(baseEntity *obj);

            };
    }
