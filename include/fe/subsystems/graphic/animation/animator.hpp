// animator.hpp
// allows the subscription to a animation with options
#pragma once
#include "fe/flatEngineExport.hpp"

#include <unordered_map>
#include <vector>

#include "fe/objectManagement/handleManager.hpp"
#include "fe/time/clock.hpp"
#include "fe/math/Vector2.hpp"

#include "animationTexture.hpp"
#include "animationActor.hpp"
#include "fe/subsystems/memory/poolAllocater.hpp"

namespace fe
    {
        class animationActor;
        struct renderObject;
        class animator : fe::handleManager<animationTexture, 0>
            {
                private:
                    std::unordered_map<fe::Handle, std::vector<animationActor*>> m_actors;
                    fe::poolAllocater<animationActor> m_animationActorPool;

                    fe::clock m_elapsedTime;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API animationActor *alloc(fe::renderObject *const object);
                    FLAT_ENGINE_API void free(animationActor *actor);

                    FLAT_ENGINE_API fe::Handle addAnimation(fe::Vector2<unsigned int> frameSize, fe::Vector2<unsigned int> animationSize, bool vertical = true, fe::Vector2<unsigned int> texturePosition = fe::Vector2<unsigned int>(0, 0));
                    FLAT_ENGINE_API void removeAnimation(fe::Handle handle);
                
                    FLAT_ENGINE_API void subscribe(animationActor *actor, fe::Handle animation);
                    FLAT_ENGINE_API void unsubscribe(animationActor *actor, fe::Handle anitmation);
                    FLAT_ENGINE_API void unsubscribe(animationActor *actor);

                    FLAT_ENGINE_API void updateTextures();
            };
    }