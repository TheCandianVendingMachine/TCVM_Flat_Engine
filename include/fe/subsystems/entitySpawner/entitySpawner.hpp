// entitySpawner.hpp
// Allows the spawning of entities from an external Lua file
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "userEntityObject.hpp"
#include "../../math/Vector2.hpp"
#include "../resourceManager/fontData.hpp"
#include "../../entity/entityModules.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <sol.hpp>
#include <SFML/Graphics/Color.hpp>

namespace fe
    {
        class gameWorld;
        class entitySpawner
            {
                private:
                    struct prefabObject
                        {
                            sol::protected_function m_onAdd;
                            sol::protected_function m_onRemove;
                            sol::protected_function m_update;
                            sol::protected_function m_postUpdate;
                            sol::protected_function m_fixedUpdate;
                            sol::protected_function m_onCollision;
                            sol::table m_entityTable;

                            std::unordered_map<fe::str, sol::protected_function> m_events;

                            fe::fontData m_font = fe::fontData();
                            sf::Color m_colour;
                            fe::lightVector2<unsigned int> m_size;
                            fe::lightVector2<unsigned int> m_colliderSize;
                            fe::lightVector2d m_colliderPosition;
                            fe::str m_collisionEvent = 0;
                            int m_connected = -1;
                            int m_zPosition = 0;
                            float m_maxSpeed = 0.f;
                            float m_mass = 1.f;
                            float m_frictionCoef = 0.f;
                            fe::entityModules m_modules = fe::entityModules::NONE;
                            bool m_solid = true;
                        };

                    std::unordered_map<std::string, prefabObject> m_prefabs;
                    fe::userEntityObject m_objects[FE_MAX_GAME_OBJECTS];
                    unsigned int m_maxObjectCount;

                    fe::gameWorld *m_world;

                public:
                    FLAT_ENGINE_API entitySpawner();
                    FLAT_ENGINE_API void setWorld(fe::gameWorld *world);

                    FLAT_ENGINE_API void createPrefab(const char *luaName);
                    FLAT_ENGINE_API fe::Handle spawn(const char *name);

                    FLAT_ENGINE_API void getPrefabs(std::vector<std::string> &prefabs) const;

            };
    }
