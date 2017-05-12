// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "spriteBatch.hpp"
#include "texturePacker.hpp"
#include "animation/animator.hpp"
#include <vector>

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class baseEntity;

        class sceneGraph : public handleManager<fe::baseEntity*>
            {
                private:
                    spriteBatch m_batch;
                    texturePacker m_textureBatch;

                    animator m_animator;

                    FLAT_ENGINE_API void onAdd(fe::baseEntity *entity);

                public:
                    FLAT_ENGINE_API void postInit();

                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

                    fe::Vector2<unsigned int> addTexture(sf::Texture *texture);
                    fe::Handle addAnimation(sf::Texture *texture, fe::Vector2<unsigned int> frameSize);

                    void subscribe(fe::animationActor *actor, fe::Handle animation);
                    void unsubscribe(fe::animationActor *actor, fe::Handle animation);

                    FLAT_ENGINE_API ~sceneGraph();

            };
    }