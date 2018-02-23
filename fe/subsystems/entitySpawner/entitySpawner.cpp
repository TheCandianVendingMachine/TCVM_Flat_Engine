#include "entitySpawner.hpp"
#include "../../feAssert.hpp"
#include "../../engine.hpp"
#include "../gameState/gameWorld.hpp"
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

fe::Handle fe::entitySpawner::spawn(const char *luaName)
    {
        FE_ASSERT(m_world, "World is Nullptr");
        
        sol::table luaTable = fe::engine::get().getScriptManager().getLuaState()[luaName];

        if (luaTable.get_type() != sol::type::table)
            {
                FE_LOG_WARNING(luaName, " is not a table");
                return -1;
            }

        fe::userEntityObject *object = &m_objects[m_maxObjectCount];
        fe::entityModules modules = fe::entityModules::NONE;
        int connected = -1;
        int zPosition = 0;
        fe::fontData font = fe::fontData();

        fe::lightVector2<unsigned int> size;
        fe::lightVector2<unsigned int> colliderSize;
        fe::lightVector2d colliderPosition;
        sf::Color colour;

        // Initialize user functions
        if (luaTable["update"].get_type() == sol::type::function)
            {
                object->setUpdate(luaTable["update"]);
            }

        if (luaTable["postUpdate"].get_type() == sol::type::function)
            {
                object->setPostUpdate(luaTable["postUpdate"]);
            }

        if (luaTable["onAdd"].get_type() == sol::type::function)
            {
                object->setOnAdd(luaTable["onAdd"]);
            }

        if (luaTable["onRemove"].get_type() == sol::type::function)
            {
                object->setOnRemove(luaTable["onRemove"]);
            }

        // Initialize entity variables
        if (luaTable["size"].get_type() == sol::type::table)
            {
                size.x = luaTable["size"]["x"].get<unsigned int>();
                size.y = luaTable["size"]["y"].get<unsigned int>();
            }

        // Initialize entity modules
        if (luaTable["sceneGraph"].get_type() == sol::type::table)
            {
                // init scene graph
                sol::table sceneGraphData = luaTable["sceneGraph"];
                if (sceneGraphData["renderType"].get<std::string>() == "font")
                    {
                        modules = modules | fe::entityModules::RENDER_TEXT;
                        font = fe::engine::get().getResourceManager<sf::Texture>()->addFont(fe::engine::get().getResourceManager<sf::Font>()->get(sceneGraphData["font"]),
                                                                                            sceneGraphData["fontID"].get<std::string>().c_str(),
                                                                                            sceneGraphData["fontSize"]);
                    }
                else
                    {
                        modules = modules | fe::entityModules::RENDER_OBJECT;
                    }

                if (sceneGraphData["colour"].get_type() == sol::type::table)
                    {
                        colour.r = sceneGraphData["colour"]["r"].get<std::uint8_t>();
                        colour.g = sceneGraphData["colour"]["g"].get<std::uint8_t>();
                        colour.b = sceneGraphData["colour"]["b"].get<std::uint8_t>();
                        colour.a = sceneGraphData["colour"]["a"].get<std::uint8_t>();
                    }

                zPosition = sceneGraphData["zPos"].get<int>();
            }

        if (luaTable["rigidBody"].get_type() == sol::type::table)
            {
                // init rigid body
                modules = modules | fe::entityModules::RIGID_BODY;
            }

        if (luaTable["collisionBody"].get_type() == sol::type::table)
            {
                // init collision body
                modules = modules | fe::entityModules::COLLISION_BODY;

                sol::table collisionData = luaTable["collisionBody"];
                if (collisionData["size"].get_type() == sol::type::table)
                    {
                        colliderSize.x = collisionData["size"]["x"].get<unsigned int>();
                        colliderSize.y = collisionData["size"]["y"].get<unsigned int>();
                    }
                else
                    {
                        colliderSize = size;
                    }

                if (collisionData["position"].get_type() == sol::type::table)
                    {
                        colliderPosition.x = collisionData["position"]["x"].get<float>();
                        colliderPosition.y = collisionData["position"]["y"].get<float>();
                    }
            }

        object->startUp(m_maxObjectCount++);
        fe::Handle objectHandle = m_world->addGameObject(new fe::baseEntity(modules, object, false), connected, font);
        fe::baseEntity *entity = m_world->getObject(objectHandle);

        entity->setSize(size);
        entity->setColour(colour);

        if ((modules & fe::entityModules::COLLISION_BODY) == 1)
            {
                entity->getCollider()->m_aabb.m_positionX = colliderPosition.x;
                entity->getCollider()->m_aabb.m_positionY = colliderPosition.y;
                entity->getCollider()->m_aabb.m_sizeX = colliderSize.x;
                entity->getCollider()->m_aabb.m_sizeY = colliderSize.y;
            }

        if ((modules & fe::entityModules::RENDER_OBJECT) == 1 || (modules & fe::entityModules::RENDER_TEXT) == 1)
            {
                entity->getRenderObject()->m_zPosition = zPosition;
            }

        return objectHandle;
    }
