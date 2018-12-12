// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#include "../flatEngineExport.hpp"
#include "../typeDefines.hpp"
#include "../objectManagement/guid.hpp"
#include "../subsystems/serializer/serializable.hpp"
#include "../math/Vector2.hpp"
#include "../subsystems/resourceManager/fontData.hpp"
#include "scriptObject.hpp"
#include "entityModules.hpp"
#include "baseEntityUtilities.hpp"
#include <unordered_map>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vector>
#include <utility>

namespace fe
    {
        struct sceneGraphObject;
        struct collider;
        class rigidBody;
        class baseGameState;
        class gameWorld;
        class userEntityObject;
        class animationActor;

        class baseEntity : public fe::guid, public fe::scriptObject
            {
                protected:
                    fe::baseEntityUtilities m_baseEntityUtility;

                    fe::sceneGraphObject *m_renderObject;
                    fe::animationActor *m_animationActor;
                    fe::rigidBody *m_rigidBody;
                    fe::collider *m_collisionBody;

                    // Component handle and whether or not its enabled
                    std::vector<std::pair<fe::Handle, bool>> m_components;

                    fe::userEntityObject *m_entityScriptObject; // object that handles updating per fe::baseEntity::update

                    fe::entityModules m_enabledModulesEnum;

                    fe::Handle m_handle;

                    std::string m_name;
                    fe::str m_tag;

                    // Local variables to keep updated for debugging. Is only used to set position on initialization
                    float m_positionX;
                    float m_positionY;
                    float m_sizeX;
                    float m_sizeY;
                    float m_rotation;
                    sf::Color m_colour;

                    bool m_killEntity;
                    bool m_enabled;
                    bool m_moved;
                    bool m_static;
                    bool m_serialize;

                    bool m_allocatedModules;

                    friend class entityWorld;
                    friend class entitySpawner;

                public:
                    baseEntity() : baseEntity(fe::entityModules::NONE, nullptr, false) {};
                    FLAT_ENGINE_API baseEntity(fe::entityModules modules, fe::userEntityObject *scriptObject, bool staticObject);
                    FLAT_ENGINE_API void initialize(fe::gameWorld &world, int connected = -1, const fe::fontData &font = fe::fontData());
                    FLAT_ENGINE_API void deinitialize(fe::gameWorld &world);

                    FLAT_ENGINE_API void onAdd(fe::gameWorld &world);
                    FLAT_ENGINE_API void onRemove(fe::gameWorld &world);

                    FLAT_ENGINE_API void enable(bool value);
                    FLAT_ENGINE_API bool getEnabled() const;

                    FLAT_ENGINE_API void enableDrawing(bool value);
                    FLAT_ENGINE_API void enablePhysics(bool value);
                    FLAT_ENGINE_API void enableCollision(bool value);

                    FLAT_ENGINE_API void kill(bool value);
                    FLAT_ENGINE_API bool isKilled() const;

                    FLAT_ENGINE_API void setHandle(fe::Handle handle);
                    FLAT_ENGINE_API fe::Handle getHandle() const;

                    FLAT_ENGINE_API void setTag(const char *tag);
                    FLAT_ENGINE_API void setTag(fe::str tag);
                    FLAT_ENGINE_API fe::str getTag() const;

                    FLAT_ENGINE_API void onDestroy(fe::baseGameState &state);

                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void fixedUpdate(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();
                    FLAT_ENGINE_API void updateModules();

                    FLAT_ENGINE_API void setScriptObject(fe::userEntityObject *obj);
                    FLAT_ENGINE_API fe::userEntityObject *getEntityObject() const;

                    FLAT_ENGINE_API void setPosition(float x, float y);
                    FLAT_ENGINE_API void setPosition(fe::Vector2d position);
                    FLAT_ENGINE_API void setPosition(fe::lightVector2d position);

                    FLAT_ENGINE_API void setSize(float x, float y);
                    FLAT_ENGINE_API void setSize(fe::Vector2d size);
                    FLAT_ENGINE_API void setSize(fe::lightVector2d size);

                    FLAT_ENGINE_API void setRotation(float degree);
                    FLAT_ENGINE_API float getRotation() const;

                    FLAT_ENGINE_API void setColour(const sf::Color colour);

                    FLAT_ENGINE_API void setName(const std::string &name);
                    FLAT_ENGINE_API std::string getName() const;

                    FLAT_ENGINE_API fe::lightVector2d getPosition() const;
                    FLAT_ENGINE_API fe::lightVector2d getSize() const;
                    FLAT_ENGINE_API sf::Color getColour() const;

                    FLAT_ENGINE_API fe::sceneGraphObject *getRenderObject() const;
                    FLAT_ENGINE_API fe::rigidBody *getRigidBody() const;
                    FLAT_ENGINE_API fe::collider *getCollider() const;
                    FLAT_ENGINE_API fe::animationActor *getActor() const;

                    FLAT_ENGINE_API fe::entityModules getModules() const;

                    FLAT_ENGINE_API fe::baseEntityUtilities &getUtilities();

                    FLAT_ENGINE_API void createModules(fe::baseGameState &currentState);

                    FLAT_ENGINE_API void addComponent(fe::Handle handle);
                    FLAT_ENGINE_API void removeComponent(fe::Handle handle);
                    FLAT_ENGINE_API void enableComponent(fe::Handle handle, bool value);
                    FLAT_ENGINE_API void enableAllComponents(bool value);
                    FLAT_ENGINE_API bool isComponentEnabled(fe::Handle handle) const;
                    FLAT_ENGINE_API const std::vector<std::pair<fe::Handle, bool>> &getAllComponents() const;

                    FLAT_ENGINE_API void enableSerialization(bool value);
                    FLAT_ENGINE_API bool isSerializable() const;

            };
    }