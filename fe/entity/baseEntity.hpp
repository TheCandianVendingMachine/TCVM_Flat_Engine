// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../subsystems/graphic/renderObject/renderObject.hpp"

namespace fe
    {
        class baseEntity
            {
                private:
                    fe::renderObject *m_renderObject;

                protected:
                public:

            };
    }