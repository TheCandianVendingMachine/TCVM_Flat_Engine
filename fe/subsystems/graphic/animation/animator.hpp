// animator.hpp
// allows the subscription to a animation with options
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "../../../objectManagement/handleManager.hpp"
#include "../../../time/clock.hpp"

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
                    FLAT_ENGINE_API fe::Handle addAnimation(sf::Vector2u frameSize, sf::Vector2u animationSize, sf::Vector2u texturePosition = sf::Vector2u(0, 0), bool vertical = true);
                    FLAT_ENGINE_API void removeAnimation(fe::Handle handle);
                
                    FLAT_ENGINE_API void subscribe(animationActor *actor, fe::Handle animation);
                    FLAT_ENGINE_API void unsubscribe(animationActor *actor, fe::Handle anitmation);

                    FLAT_ENGINE_API void updateTextures();
            };
    }