// entityWorld.hpp
// All entities in the world are stored in here. Manages addition, removal, and serializaiton
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../serializer/serializable.hpp"
#include "../resourceManager/fontData.hpp"
#include "../memory/poolAllocater.hpp"
#include "../../entity/baseEntity.hpp"
#include "../../entity/entityModules.hpp"

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
                            bool m_enabled;

                            FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                            FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
                        };

                    fe::poolAllocater<fe::baseEntity> m_entityAllocater;

                    fe::gameWorld &m_gameWorld;

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

                    FLAT_ENGINE_API void serialize(fe::serializerID &serializer);
                    FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

            };
    }
