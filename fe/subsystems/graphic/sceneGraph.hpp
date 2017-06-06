// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../serializer/serializerID.hpp"
#include "spriteBatch.hpp"
#include "animation/animator.hpp"
#include "tileMap.hpp"
#include <vector>

namespace sf
    {
        class RenderTarget;
        class Texture;
    }

namespace fe
    {
        class baseEntity;
        class baseGameState;

        class sceneGraph : public handleManager<fe::baseEntity*>
            {
                private:
                    fe::serializerID m_serializer;

                    spriteBatch m_batch;
                    animator m_animator;
                    tileMap m_tileMap;

                    FLAT_ENGINE_API void onAdd(fe::baseEntity* object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();
                    FLAT_ENGINE_API void cullObjects(fe::baseGameState &state);

                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

                    FLAT_ENGINE_API fe::tileMap &getTileMap();

                    FLAT_ENGINE_API fe::Handle addTexture(const char *filepath, const char *name);
                    FLAT_ENGINE_API fe::Handle addAnimation(const char *filepath, const char *name, fe::Vector2<unsigned int> frameSize);

                    FLAT_ENGINE_API fe::Handle addTexture(const sf::Texture *texture, fe::Vector2<unsigned int> offset);
                    FLAT_ENGINE_API fe::Handle addAnimation(const sf::Texture *texture, fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> frameSize);

                    FLAT_ENGINE_API void subscribe(fe::animationActor *actor, fe::Handle animation);
                    FLAT_ENGINE_API void unsubscribe(fe::animationActor *actor, fe::Handle animation);
                    FLAT_ENGINE_API void unsubscribe(fe::animationActor *actor);

                    // save all scene data to file
                    FLAT_ENGINE_API void save(const char *filepath);
                    // load all scene data from file
                    FLAT_ENGINE_API void load(const char *filepath);

                    FLAT_ENGINE_API ~sceneGraph();

            };
    }