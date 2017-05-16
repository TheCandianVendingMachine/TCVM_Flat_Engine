// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "spriteBatch.hpp"
#include "animation/animator.hpp"
#include <vector>

namespace sf
    {
        class RenderTarget;
        class Texture;
    }

namespace fe
    {
        class baseEntity;

        class sceneGraph : public handleManager<fe::baseEntity*>
            {
                private:
                    spriteBatch m_batch;
                    animator m_animator;

                public:
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

                    FLAT_ENGINE_API fe::Handle addTexture(const char *filepath, const char *name);
                    FLAT_ENGINE_API fe::Handle addAnimation(const char *filepath, const char *name, fe::Vector2<unsigned int> frameSize);

                    FLAT_ENGINE_API fe::Handle addTexture(const sf::Texture *texture, fe::Vector2<unsigned int> offset);
                    FLAT_ENGINE_API fe::Handle addAnimation(const sf::Texture *texture, fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> frameSize);

                    FLAT_ENGINE_API void subscribe(fe::animationActor *actor, fe::Handle animation);
                    FLAT_ENGINE_API void unsubscribe(fe::animationActor *actor, fe::Handle animation);
                    FLAT_ENGINE_API void unsubscribe(fe::animationActor *actor);

                    FLAT_ENGINE_API ~sceneGraph();

            };
    }