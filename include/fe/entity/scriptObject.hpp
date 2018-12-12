// scriptObject.hpp
// A basic object that the scripting system uses to gain knowledge about objects
#pragma once
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

                    FLAT_ENGINE_API void scriptObjectSetTag(const char *tag);
                    FLAT_ENGINE_API unsigned int scriptObjectGetTag() const;

                    FLAT_ENGINE_API void scriptObjectSetOrigin(float x, float y);

                    FLAT_ENGINE_API std::string scriptObjectGetName();

                    FLAT_ENGINE_API void scriptObjectSetVelocity(fe::Vector2d velocity);
                    FLAT_ENGINE_API void scriptObjectSetVelocity(float x, float y);

                    FLAT_ENGINE_API void scriptObjectSetForce(fe::Vector2d force);
                    FLAT_ENGINE_API void scriptObjectSetForce(float x, float y)
                        ;
                    FLAT_ENGINE_API void scriptObjectApplyForce(fe::Vector2d force);
                    FLAT_ENGINE_API void scriptObjectApplyForce(float x, float y);

                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetPosition();
                    FLAT_ENGINE_API void scriptObjectSetPosition(fe::Vector2d pos);
                    FLAT_ENGINE_API void scriptObjectSetPosition(float x, float y);

                    FLAT_ENGINE_API void scriptObjectSetRotation(float degree);
                    FLAT_ENGINE_API float scriptObjectGetRotation();

                    FLAT_ENGINE_API void scriptObjectDestroy();

                    FLAT_ENGINE_API void scriptObjectSetColour(int r, int g, int b, int a = 255);

                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetVelocity();
                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetNormalForce();
                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetForce();

                    FLAT_ENGINE_API fe::Vector2d scriptObjectGetDirection();

                    FLAT_ENGINE_API void scriptObjectMoveToPosition(fe::Vector2d pos);
                    FLAT_ENGINE_API void scriptObjectMoveToPosition(float x, float y);

                    FLAT_ENGINE_API float scriptObjectGetSlowdownDistance();
                    FLAT_ENGINE_API void scriptObjectSetSlowdownDistance(float distance);

                    FLAT_ENGINE_API bool scriptObjectHasArrived();
                    FLAT_ENGINE_API bool scriptObjectIsTargeted();

                    FLAT_ENGINE_API void scriptObjectSetAnimationFrame(unsigned int x, unsigned int y);
                    FLAT_ENGINE_API void scriptObjectPlayAnimationSequence(const char *sequence);
                    FLAT_ENGINE_API bool scriptObjectAnimationPlayingSequence() const;
                    FLAT_ENGINE_API bool scriptObjectAnimationPlaying() const;

            };
    }
