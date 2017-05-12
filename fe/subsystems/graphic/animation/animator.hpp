// animator.hpp
// allows the subscription to a animation with options
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

#include <unordered_map>
#include <vector>

#include "../../../objectManagement/handleManager.hpp"
#include "../../../time/clock.hpp"
#include "../../../math/Vector2.hpp"

namespace sf
    {
        class Texture;
    }

namespace fe
    {
        class animationTexture;
        class animationActor;
        class animator : fe::handleManager<animationTexture>
            {
                private:
                    std::unordered_map<fe::Handle, std::vector<animationActor*>> m_actors;
                    fe::clock m_elapsedTime;

                public:
                    FLAT_ENGINE_API fe::Handle addAnimation(fe::Vector2<unsigned int> frameSize, fe::Vector2<unsigned int> animationSize, fe::Vector2<unsigned int> texturePosition = fe::Vector2<unsigned int>(0, 0), bool vertical = true);
                    FLAT_ENGINE_API void removeAnimation(fe::Handle handle);
                
                    FLAT_ENGINE_API void subscribe(animationActor *actor, fe::Handle animation);
                    FLAT_ENGINE_API void unsubscribe(animationActor *actor, fe::Handle anitmation);

                    FLAT_ENGINE_API void updateTextures();
            };
    }