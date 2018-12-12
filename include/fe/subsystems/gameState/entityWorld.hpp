// entityWorld.hpp
// All entities in the world are stored in here. Manages addition, removal, and serializaiton
#pragma once

#include "fe/flatEngineExport.hpp"
#include "fe/objectManagement/handleManager.hpp"
#include "fe/subsystems/serializer/serializable.hpp"
#include "fe/subsystems/resourceManager/fontData.hpp"
#include "fe/subsystems/memory/poolAllocater.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/entity/entityModules.hpp"
#include "fe/subsystems/graphic/animation/animator.hpp"
#include <stack>
#include <vector>

namespace fe
    {
        class broadphaseAbstract;
        class collisionWorld;
        class gameWorld;
        class userEntityObject;

        class entityWorld : protected fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>, public fe::serializable
            {
                private:
                    struct entityRepresentation : fe::serializable
                        {
                            std::string m_name;
                            float m_positionX;
                            float m_positionY;
                            fe::Handle m_handle;
                            std::vector<std::pair<fe::Handle, bool>> m_components;
                            bool m_enabled;

                            fe::gameWorld &m_gameWorld;

                            entityRepresentation(fe::gameWorld &gameWorld) : m_gameWorld(gameWorld) {}

                            FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                            FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
                        };

                    fe::poolAllocater<fe::baseEntity> m_entityAllocater;
                    std::stack<fe::baseEntity*> m_newEntities;

                    fe::gameWorld &m_gameWorld;
                    fe::animator m_animator;

                    FLAT_ENGINE_API void onAdd(fe::baseEntity **object, fe::Handle objectHandle);
                    FLAT_ENGINE_API void onRemove(fe::baseEntity **object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API entityWorld(fe::gameWorld &gameWorld);

                    FLAT_ENGINE_API void startUp();

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(collisionWorld *collisionWorld, broadphaseAbstract *dynamicBroadphase);
                    FLAT_ENGINE_API void fixedUpdate(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API fe::baseEntity *addGameObject(fe::entityModules modules, fe::userEntityObject *scriptObject, int connected = -1, const fe::fontData &data = fe::fontData());

                    FLAT_ENGINE_API void clearAllObjects();
                    FLAT_ENGINE_API void removeObject(fe::Handle handle);
                    FLAT_ENGINE_API fe::baseEntity *getObject(fe::Handle handle) const;
                    FLAT_ENGINE_API void getAllObjects(std::vector<fe::baseEntity*> &objects);
                    FLAT_ENGINE_API void getAllObjectsByTag(std::vector<fe::baseEntity*> &objects, const char *tag);
                    FLAT_ENGINE_API void getAllObjectsByTag(std::vector<fe::baseEntity*> &objects, fe::str tag);

                    FLAT_ENGINE_API void serialize(fe::serializerID &serializer);
                    FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

                    FLAT_ENGINE_API fe::animator &getAnimator();

                    FLAT_ENGINE_API bool isEntity(void *ptr) const;

            };
    }
