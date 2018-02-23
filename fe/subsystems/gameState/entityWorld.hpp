// entityWorld.hpp
// All entities in the world are stored in here. Manages addition, removal, and serializaiton
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../serializer/serializable.hpp"
#include "../resourceManager/fontData.hpp"

namespace fe
    {
        class broadphaseAbstract;
        class collisionWorld;
        class gameWorld;
        class baseEntity;

        class entityWorld : protected fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>, public fe::serializable
            {
                private:
                    fe::gameWorld &m_gameWorld;

                    FLAT_ENGINE_API void onAdd(fe::baseEntity **object, fe::Handle objectHandle);
                    FLAT_ENGINE_API void onRemove(fe::baseEntity **object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API entityWorld(fe::gameWorld &gameWorld);

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(collisionWorld *collisionWorld, broadphaseAbstract *dynamicBroadphase);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API fe::baseEntity *addGameObject(fe::baseEntity *entity, int connected = -1, fe::fontData &data = fe::fontData());

                    FLAT_ENGINE_API void clearAllObjects();
                    FLAT_ENGINE_API void removeObject(fe::Handle handle);
                    FLAT_ENGINE_API fe::baseEntity *getObject(fe::Handle handle) const;

                    FLAT_ENGINE_API void serialize(fe::serializerID &serializer);
                    FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

            };
    }