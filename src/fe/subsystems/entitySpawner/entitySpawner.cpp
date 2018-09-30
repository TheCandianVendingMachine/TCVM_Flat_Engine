#include "fe/subsystems/entitySpawner/entitySpawner.hpp"
#include "fe/feAssert.hpp"
#include "fe/engine.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/gameState/gameWorld.hpp"
#include "fe/subsystems/physics/rigidBody.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"
#include "fe/subsystems/resourceManager/resourceManager.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"

fe::entitySpawner::entitySpawner() : 
    m_world(nullptr),
    m_maxObjectCount(0)
    {
    }

void fe::entitySpawner::setWorld(fe::gameWorld *world)
    {
        m_world = world;
    }

fe::prefabObject &fe::entitySpawner::createPrefab(const char *luaName)
    {
        sol::table luaTable = fe::engine::get().getScriptManager().getTable(luaName);
        prefabObject prefab;
        prefab.m_entityTable = luaTable;

        if (luaTable.get_type() != sol::type::table)
            {
                FE_LOG_WARNING(luaName, " is not a valid entity");
                return prefab;
            }

        // Initialize user functions
        if (luaTable["update"].get_type() == sol::type::function)
            {
                prefab.m_update = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(luaName, "update");
            }

        if (luaTable["postUpdate"].get_type() == sol::type::function)
            {
                prefab.m_postUpdate = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(luaName, "postUpdate");
            }

        if (luaTable["fixedUpdate"].get_type() == sol::type::function)
            {
                prefab.m_fixedUpdate = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(luaName, "fixedUpdate");
            }

        if (luaTable["onAdd"].get_type() == sol::type::function)
            {
                prefab.m_onAdd = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(luaName, "onAdd");
            }

        if (luaTable["onRemove"].get_type() == sol::type::function)
            {
                prefab.m_onRemove = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(luaName, "onRemove");
            }

        if (luaTable["events"].get_type() == sol::type::table)
            {
                luaTable["events"].get<sol::table>().for_each([&prefab, &luaName](sol::object key, sol::object value)
                    {
                        if (value.get_type() == sol::type::function)
                            {
                                prefab.m_events[FE_STR(key.as<std::string>().c_str())] = 
                                    &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(std::string(luaName) + "/events", key.as<std::string>());
                            }
                    });
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
                sol::table sceneGraphData = fe::engine::get().getScriptManager().getValueFromTable(luaTable, "sceneGraph");
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

                if (sceneGraphData["texture"].get_type() == sol::type::table)
                    {
                        prefab.m_textureID = FE_STR(sceneGraphData["texture"]["name"].get<std::string>().c_str());

                        if (sceneGraphData["offset"].get_type() == sol::type::table)
                            {
                                prefab.m_textureOffset.x = sceneGraphData["offset"]["x"].get<unsigned int>();
                                prefab.m_textureOffset.y = sceneGraphData["offset"]["y"].get<unsigned int>();
                            }
                        else
                            {
                                prefab.m_textureOffset.x = 0;
                                prefab.m_textureOffset.y = 0;
                            }

                        sf::Texture *tex = fe::engine::get().getResourceManager<sf::Texture>()->getTexture(prefab.m_textureID);
                        if (!tex)
                            {
                                std::string reason = "Entity texture is invalid! Ent='" + std::string(luaName) + "'";
                                fe::engine::crash(reason.c_str());
                            }
                        sf::Vector2u textureSize = tex->getSize();
                        prefab.m_textureSize.x = textureSize.x;
                        prefab.m_textureSize.y = textureSize.y;
                        if (sceneGraphData["size"].get_type() == sol::type::table)
                            {
                                if (sceneGraphData["x"].get_type() == sol::type::number)
                                    {
                                        prefab.m_textureSize.x = sceneGraphData["x"].get<unsigned int>();
                                    }

                                if (sceneGraphData["y"].get_type() == sol::type::number)
                                    {
                                        prefab.m_textureSize.y = sceneGraphData["y"].get<unsigned int>();
                                    }
                            }
                    }

                prefab.m_zPosition = sceneGraphData["zPos"].get<int>();

                if (sceneGraphData["animation"].get_type() == sol::type::table)
                    {
                        sol::table animationData = sceneGraphData["animation"];
                        prefab.m_modules = prefab.m_modules | fe::entityModules::ANIMATION;

                        if (animationData["frameSize"].get_type() == sol::type::table)
                            {
                                prefab.m_animationFrameSize.x = animationData["frameSize"]["x"].get<unsigned int>();
                                prefab.m_animationFrameSize.y = animationData["frameSize"]["y"].get<unsigned int>();
                            }

                        if (animationData["totalSize"].get_type() == sol::type::table)
                            {
                                prefab.m_animationTotalSize.x = animationData["totalSize"]["x"].get<unsigned int>();
                                prefab.m_animationTotalSize.x = animationData["totalSize"]["y"].get<unsigned int>();
                            }

                        if (animationData["textureOffset"].get_type() == sol::type::table)
                            {
                                prefab.m_animationTextureOffset.x = animationData["textureOffset"]["x"].get<unsigned int>();
                                prefab.m_animationTextureOffset.x = animationData["textureOffset"]["y"].get<unsigned int>();
                            }

                        if (animationData["vertical"].get_type() == sol::type::boolean)
                            {
                                prefab.m_animationIsVertical = animationData["vertical"].get<bool>();
                            }

                        if (animationData["startFrame"].get_type() == sol::type::number)
                            {
                                prefab.m_animationStartFrame = animationData["startFrame"].get<unsigned int>();
                            }

                        if (animationData["endFrame"].get_type() == sol::type::number)
                            {
                                prefab.m_animationEndFrame = animationData["endFrame"].get<unsigned int>();
                            }

                        if (animationData["frameSpeed"].get_type() == sol::type::number)
                            {
                                prefab.m_animationFrameSpeed = animationData["frameSpeed"].get<unsigned int>();
                            }
                    }
            }

        if (luaTable["rigidBody"].get_type() == sol::type::table)
            {
                // init rigid body
                prefab.m_modules = prefab.m_modules | fe::entityModules::RIGID_BODY;

                sol::table rigidBodyData = fe::engine::get().getScriptManager().getValueFromTable(luaTable, "rigidBody");
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

                sol::table collisionData = fe::engine::get().getScriptManager().getValueFromTable(luaTable, "collisionBody");
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
                else
                    {
                        prefab.m_colliderPosition.x = 0.f;
                        prefab.m_colliderPosition.y = 0.f;
                    }

                if (collisionData["event_on_collision"].get_type() == sol::type::string)
                    {
                        prefab.m_collisionEvent = FE_STR(collisionData["event_on_collision"].get<std::string>().c_str());
                    }

                if (collisionData["solid"].get_type() == sol::type::boolean)
                    {
                        prefab.m_solid = collisionData["solid"].get<bool>();
                    }

                if (collisionData["on_collision"].get_type() == sol::type::function)
                    {
                        prefab.m_onCollision = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(std::string(luaName) + "/collisionBody", "on_collision");
                    }

                if (collisionData["group"].get_type() == sol::type::string)
                    {
                        prefab.m_collisionGroup = FE_STR(collisionData["group"].get<std::string>().c_str());
                    }
            }

        if (luaTable["components"].get_type() == sol::type::table)
            {
                sol::table components = luaTable["components"];
                for (auto &value : components)
                    {
                        if (value.second.get_type() == sol::type::table)
                            {
                                prefab.m_components.emplace_back(value.first.as<std::string>(), value.second.as<sol::table>());
                            }
                    }
            }

        for (auto &value : luaTable)
            {
                if (value.second.get_type() == sol::type::function)
                    {
                        prefab.addUserFunction(luaName, value.first.as<std::string>().c_str());
                    }
            }

        m_prefabs[luaName] = prefab;

        return m_prefabs[luaName];
    }

fe::Handle fe::entitySpawner::spawn(const char *luaName)
    {
        FE_ASSERT(m_world, "World is Nullptr");
        
        fe::userEntityObject *object = nullptr;
        for (unsigned int i = 0; i < FE_MAX_GAME_OBJECTS; i++)
            {
                if (!m_objects[i].active())
                    {
                        object = &m_objects[i];
                        object->startUp(i);
                        break;
                    }
            }

        FE_ASSERT(object, "userEntityObject is nullptr!");

        if (m_prefabs.find(luaName) == m_prefabs.end())
            {
                createPrefab(luaName);
            }
        const prefabObject &prefab = m_prefabs[luaName];
        
        *object = prefab;

        fe::Handle objectHandle = m_world->addGameObject(prefab.m_modules, object, prefab.m_connected, prefab.m_font);
        fe::baseEntity *entity = m_world->getObject(objectHandle);

        entity->setSize(prefab.m_size);
        entity->setColour(prefab.m_colour);

        entity->setName(luaName);
        entity->setEntityDefinition(prefab.m_entityTable);

        if (prefab.m_modules & fe::entityModules::COLLISION_BODY)
            {
                entity->getCollider()->m_aabb.m_offsetX = prefab.m_colliderPosition.x;
                entity->getCollider()->m_aabb.m_offsetY = prefab.m_colliderPosition.y;
                entity->getCollider()->m_aabb.m_sizeX = prefab.m_colliderSize.x;
                entity->getCollider()->m_aabb.m_sizeY = prefab.m_colliderSize.y;
                entity->getCollider()->m_eventOnCollision = prefab.m_collisionEvent;
                entity->getCollider()->m_solid = prefab.m_solid;
                entity->getCollider()->m_collisionGroup = prefab.m_collisionGroup;
                if (prefab.m_onCollision) 
                    {
                        entity->getCollider()->m_collisionCallback = [prefab, entity](const fe::collisionData &data) { prefab.m_onCollision->call(static_cast<fe::scriptObject*>(entity), data); };
                    }
            }

        if ((prefab.m_modules & fe::entityModules::RENDER_OBJECT) || (prefab.m_modules & fe::entityModules::RENDER_TEXT))
            {
                entity->getRenderObject()->m_zPosition = prefab.m_zPosition;
                m_world->getSceneGraph().setZOrder(entity->getRenderObject(), prefab.m_zPosition);

                fe::Vector2<unsigned int> offset = fe::engine::get().getResourceManager<sf::Texture>()->getTexturePosition(prefab.m_textureID);
                entity->getRenderObject()->m_texCoords[0] = offset.x + prefab.m_textureOffset.x;
                entity->getRenderObject()->m_texCoords[1] = offset.y + prefab.m_textureOffset.y;

                entity->getRenderObject()->m_texCoords[2] = prefab.m_textureSize.x;
                entity->getRenderObject()->m_texCoords[3] = prefab.m_textureSize.y;

                if (prefab.m_modules & fe::entityModules::ANIMATION)
                    {
                        fe::lightVector2<unsigned int> frameSize = prefab.m_animationFrameSize;
                        fe::lightVector2<unsigned int> totalSize = prefab.m_animationTotalSize;
                        fe::lightVector2<unsigned int> textureOffset = offset + prefab.m_animationTextureOffset;
                        bool vertical = prefab.m_animationIsVertical;

                        fe::Handle animation = m_world->getEntityWorld().getAnimator().addAnimation(frameSize, totalSize, vertical, textureOffset);
                        m_world->getEntityWorld().getAnimator().subscribe(entity->getActor(), animation);

                        entity->getActor()->setStartFrame(prefab.m_animationStartFrame);
                        entity->getActor()->setEndFrame(prefab.m_animationEndFrame);
                        entity->getActor()->setFrameSpeed(prefab.m_animationFrameSpeed);
                        entity->getActor()->play(false);
                    }
            }

        if (prefab.m_modules & fe::entityModules::RIGID_BODY)
            {
                entity->getRigidBody()->setMaxSpeed(prefab.m_maxSpeed);
                entity->getRigidBody()->setMass(prefab.m_mass);
                entity->getRigidBody()->setFrictionCoefficient(prefab.m_frictionCoef);
            }

        for (auto &component : prefab.m_components)
            {
                std::string path = luaName + std::string("/components/") + component.first;
                m_world->getComponentManager().addComponentToObject(entity, luaName, component.first, path, component.second);
            }

        fe::gameEvent createEvent(FE_STR((std::string("spawn_") + luaName).c_str()), 1);
        createEvent.args[0].arg.TYPE_UINTEGER = objectHandle;
        createEvent.args[0].argType = fe::gameEventArgument::type::TYPE_UINT;

        fe::engine::get().getEventSender().sendGlobal(createEvent);

        return objectHandle;
    }

void fe::entitySpawner::despawn(fe::Handle object)
    {
        m_objects[m_world->getObject(object)->getEntityObject()->index()].shutDown();
    }

void fe::entitySpawner::getPrefabs(std::vector<std::string> &prefabs) const
    {
        for (auto &prefab : m_prefabs)
            {
                prefabs.push_back(prefab.first);
            }
    }
