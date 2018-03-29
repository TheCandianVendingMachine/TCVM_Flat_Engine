#include "entitySpawner.hpp"
#include "../../feAssert.hpp"
#include "../../engine.hpp"
#include "../../entity/baseEntity.hpp"
#include "../gameState/gameWorld.hpp"
#include "../physics/rigidBody.hpp"
#include "../scripting/scriptManager.hpp"
#include "../resourceManager/resourceManager.hpp"

fe::entitySpawner::entitySpawner() : 
    m_world(nullptr),
    m_maxObjectCount(0)
    {
    }

void fe::entitySpawner::setWorld(fe::gameWorld *world)
    {
        m_world = world;
    }

void fe::entitySpawner::createPrefab(const char *luaName)
    {
        sol::table luaTable = fe::engine::get().getScriptManager().getLuaState()[luaName];
        prefabObject prefab;

        if (luaTable.get_type() != sol::type::table)
            {
                FE_LOG_WARNING(luaName, " is not a valid entity");
                return;
            }

        // Initialize user functions
        if (luaTable["update"].get_type() == sol::type::function)
            {
                prefab.m_update = luaTable["update"];
            }

        if (luaTable["postUpdate"].get_type() == sol::type::function)
            {
                prefab.m_postUpdate = luaTable["postUpdate"];
            }

        if (luaTable["onAdd"].get_type() == sol::type::function)
            {
                prefab.m_onAdd = luaTable["onAdd"];
            }

        if (luaTable["onRemove"].get_type() == sol::type::function)
            {
                prefab.m_onRemove = luaTable["onRemove"];
            }
        
        // Initialize entity variables
        if (luaTable["size"].get_type() == sol::type::table)
            {
                prefab.m_size.x = luaTable["size"]["x"].get<unsigned int>();
                prefab.m_size.y = luaTable["size"]["y"].get<unsigned int>();
            }

        // Initialize entity modules
        if (luaTable["sceneGraph"].get_type() == sol::type::table)
            {
                // init scene graph
                sol::table sceneGraphData = luaTable["sceneGraph"];
                if (sceneGraphData["renderType"].get<std::string>() == "font")
                    {
                        prefab.m_modules = prefab.m_modules | fe::entityModules::RENDER_TEXT;
                        prefab.m_font = fe::engine::get().getResourceManager<sf::Texture>()->addFont(   fe::engine::get().getResourceManager<sf::Font>()->get(sceneGraphData["font"]),
                                                                                                        sceneGraphData["fontID"].get<std::string>().c_str(),
                                                                                                        sceneGraphData["fontSize"]);
                    }
                else
                    {
                        prefab.m_modules = prefab.m_modules | fe::entityModules::RENDER_OBJECT;
                    }

                if (sceneGraphData["colour"].get_type() == sol::type::table)
                    {
                        prefab.m_colour.r = sceneGraphData["colour"]["r"].get<std::uint8_t>();
                        prefab.m_colour.g = sceneGraphData["colour"]["g"].get<std::uint8_t>();
                        prefab.m_colour.b = sceneGraphData["colour"]["b"].get<std::uint8_t>();
                        prefab.m_colour.a = sceneGraphData["colour"]["a"].get<std::uint8_t>();
                    }

                prefab.m_zPosition = sceneGraphData["zPos"].get<int>();
            }

        if (luaTable["rigidBody"].get_type() == sol::type::table)
            {
                // init rigid body
                prefab.m_modules = prefab.m_modules | fe::entityModules::RIGID_BODY;

                sol::table rigidBodyData = luaTable["rigidBody"];
                if (rigidBodyData["maxSpeed"].get_type() == sol::type::number)
                    {
                        prefab.m_maxSpeed = rigidBodyData["maxSpeed"].get<float>();
                    }

                if (rigidBodyData["mass"].get_type() == sol::type::number)
                    {
                        prefab.m_mass = rigidBodyData["mass"].get<float>();
                    }
                
                if (rigidBodyData["friction"].get_type() == sol::type::number)
                    {
                        prefab.m_frictionCoef = rigidBodyData["friction"].get<float>();
                    }
            }

        if (luaTable["collisionBody"].get_type() == sol::type::table)
            {
                // init collision body
                prefab.m_modules = prefab.m_modules | fe::entityModules::COLLISION_BODY;

                sol::table collisionData = luaTable["collisionBody"];
                if (collisionData["size"].get_type() == sol::type::table)
                    {
                        prefab.m_colliderSize.x = collisionData["size"]["x"].get<unsigned int>();
                        prefab.m_colliderSize.y = collisionData["size"]["y"].get<unsigned int>();
                    }
                else
                    {
                        prefab.m_colliderSize = prefab.m_size;
                    }

                if (collisionData["position"].get_type() == sol::type::table)
                    {
                        prefab.m_colliderPosition.x = collisionData["position"]["x"].get<float>();
                        prefab.m_colliderPosition.y = collisionData["position"]["y"].get<float>();
                    }
            }

        m_prefabs[luaName] = prefab;
    }

fe::Handle fe::entitySpawner::spawn(const char *luaName)
    {
        FE_ASSERT(m_world, "World is Nullptr");
        
        fe::userEntityObject *object = &m_objects[m_maxObjectCount];

        if (m_prefabs.find(luaName) == m_prefabs.end())
            {
                createPrefab(luaName);
            }
        const prefabObject &prefab = m_prefabs[luaName];
        
        object->startUp(m_maxObjectCount++);
        object->setOnAdd(prefab.m_onAdd);
        object->setOnRemove(prefab.m_onRemove);
        object->setUpdate(prefab.m_update);
        object->setPostUpdate(prefab.m_postUpdate);

        fe::Handle objectHandle = m_world->addGameObject(prefab.m_modules, object, prefab.m_connected, prefab.m_font);
        fe::baseEntity *entity = m_world->getObject(objectHandle);

        entity->setSize(prefab.m_size);
        entity->setColour(prefab.m_colour);

        if (prefab.m_modules & fe::entityModules::COLLISION_BODY)
            {
                entity->getCollider()->m_aabb.m_positionX = prefab.m_colliderPosition.x;
                entity->getCollider()->m_aabb.m_positionY = prefab.m_colliderPosition.y;
                entity->getCollider()->m_aabb.m_sizeX = prefab.m_colliderSize.x;
                entity->getCollider()->m_aabb.m_sizeY = prefab.m_colliderSize.y;
            }

        if ((prefab.m_modules & fe::entityModules::RENDER_OBJECT) || (prefab.m_modules & fe::entityModules::RENDER_TEXT))
            {
                entity->getRenderObject()->m_zPosition = prefab.m_zPosition;
            }

        if (prefab.m_modules & fe::entityModules::RIGID_BODY)
            {
                entity->getRigidBody()->setMaxSpeed(prefab.m_maxSpeed);
                entity->getRigidBody()->setMass(prefab.m_mass);
                entity->getRigidBody()->setFrictionCoefficient(prefab.m_frictionCoef);
            }

        entity->setName(luaName);
        return objectHandle;
    }

void fe::entitySpawner::getPrefabs(std::vector<std::string> &prefabs) const
    {
        for (auto &prefab : m_prefabs)
            {
                prefabs.push_back(prefab.first);
            }
    }
