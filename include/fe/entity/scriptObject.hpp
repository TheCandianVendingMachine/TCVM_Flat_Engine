// scriptObject.hpp
// A basic object that the scripting system uses to gain knowledge about objects
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../math/Vector2.hpp"
#include <sol.hpp>

namespace fe
    {
        class baseEntity;
        class scriptObject
            {
                private:
                    baseEntity *const m_entity;
                    sol::table m_entityDefinition;

                public:
                    FLAT_ENGINE_API scriptObject(baseEntity *obj);
                    FLAT_ENGINE_API void setEntityDefinition(sol::table table);
                    FLAT_ENGINE_API fe::baseEntity *getBaseEntity() const;

                    FLAT_ENGINE_API std::string scriptObjectGetName();

                    FLAT_ENGINE_API void scriptObjectSetVelocity(float x, float y);
                    FLAT_ENGINE_API void scriptObjectSetForce(float x, float y);
                    FLAT_ENGINE_API void scriptObjectApplyForce(float x, float y);
                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetPosition();

                    FLAT_ENGINE_API void scriptObjectDestroy();

                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetVelocity();
                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetNormalForce();
                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetForce();

            };
    }
