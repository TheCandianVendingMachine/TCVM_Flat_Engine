// entitySpawner.hpp
// Allows the spawning of entities from an external Lua file
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "userEntityObject.hpp"
#include "prefabObject.hpp"
#include <string>
#include <vector>

namespace fe
    {
        class gameWorld;
        class entitySpawner
            {
                private:
                    std::unordered_map<std::string, fe::prefabObject> m_prefabs;
                    fe::userEntityObject m_objects[FE_MAX_GAME_OBJECTS];
                    unsigned int m_maxObjectCount;

                    fe::gameWorld *m_world;

                public:
                    FLAT_ENGINE_API entitySpawner();
                    FLAT_ENGINE_API void setWorld(fe::gameWorld *world);

                    FLAT_ENGINE_API fe::prefabObject &createPrefab(const char *luaName);
                    FLAT_ENGINE_API fe::Handle spawn(const char *name);
                    FLAT_ENGINE_API void despawn(fe::Handle object);

                    FLAT_ENGINE_API void getPrefabs(std::vector<std::string> &prefabs) const;

            };
    }
