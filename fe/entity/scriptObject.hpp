// scriptObject.hpp
// A basic object that the scripting system uses to gain knowledge about objects
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../math/Vector2.hpp"

namespace fe
    {
        class baseEntity;
        class scriptObject
            {
                private:
                    baseEntity const *m_entity;

                public:
                    FLAT_ENGINE_API scriptObject(baseEntity *obj);

                    FLAT_ENGINE_API void applyForce(float x, float y);
                    FLAT_ENGINE_API fe::Vector2d getPosition();

            };
    }
