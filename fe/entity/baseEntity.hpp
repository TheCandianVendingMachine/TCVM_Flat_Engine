// entity.hpp
// base class for all entities in the game world. All objects taht are on scene are defined as an entity
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../typeDefines.hpp"
#include "../objectManagement/guid.hpp"
#include "../subsystems/serializer/serializable.hpp"
#include "../math/Vector2.hpp"
#include "../subsystems/resourceManager/fontData.hpp"
#include "scriptObject.hpp"
#include "entityModules.hpp"
#include <SFML/Graphics/Color.hpp>
#include <string>

namespace fe
    {
        struct sceneGraphObject;
        struct collider;
        class rigidBody;
        class baseGameState;
        class gameWorld;
        class userEntityObject;

        class baseEntity : public fe::guid, private fe::scriptObject
            {
                protected:
                    fe::sceneGraphObject *m_renderObject;
                    fe::rigidBody *m_rigidBody;
                    fe::collider *m_collisionBody;

                    fe::userEntityObject *m_entityScriptObject; // object that handles updating per fe::baseEntity::update

                    fe::entityModules m_enabledModulesEnum;

                    fe::Handle m_handle;

                    std::string m_name;

                    // Local variables to keep updated for debugging. Is only used to set position on initialization
                    float m_positionX;
                    float m_positionY;
                    float m_sizeX;
                    float m_sizeY;
                    sf::Color m_colour;

                    bool m_killEntity;
                    bool m_enabled;
                    bool m_moved;
                    bool m_static;

                    bool m_allocatedModules;

                    friend class entityWorld;

                public:
                    baseEntity() : baseEntity(fe::entityModules::NONE, nullptr, false) {};
                    FLAT_ENGINE_API baseEntity(fe::entityModules modules, fe::userEntityObject *scriptObject, bool staticObject);
                    FLAT_ENGINE_API void initialize(fe::gameWorld &world, int connected = -1, const fe::fontData &font = fe::fontData());
                    FLAT_ENGINE_API void deinitialize(fe::gameWorld &world);

                    FLAT_ENGINE_API void onAdd(fe::gameWorld &world);
                    FLAT_ENGINE_API void onRemove(fe::gameWorld &world);

                    FLAT_ENGINE_API void enable(bool value);
                    FLAT_ENGINE_API bool getEnabled() const;

                    FLAT_ENGINE_API void kill(bool value);
                    FLAT_ENGINE_API bool isKilled() const;

                    FLAT_ENGINE_API void setHandle(fe::Handle handle);
                    FLAT_ENGINE_API fe::Handle getHandle() const;

                    FLAT_ENGINE_API void enableCollision(bool value);

                    FLAT_ENGINE_API void onDestroy(fe::baseGameState &state);

                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void postUpdate();
                    FLAT_ENGINE_API void updateModules();

                    FLAT_ENGINE_API void setScriptObject(fe::userEntityObject *obj);

                    FLAT_ENGINE_API void setPosition(float x, float y);
                    FLAT_ENGINE_API void setPosition(fe::Vector2d position);
                    FLAT_ENGINE_API void setPosition(fe::lightVector2d position);

                    FLAT_ENGINE_API void setSize(float x, float y);
                    FLAT_ENGINE_API void setSize(fe::Vector2d size);
                    FLAT_ENGINE_API void setSize(fe::lightVector2d size);

                    FLAT_ENGINE_API void setColour(const sf::Color colour);

                    FLAT_ENGINE_API void setName(const std::string &name);
                    FLAT_ENGINE_API std::string getName() const;

                    FLAT_ENGINE_API fe::lightVector2d getPosition() const;
                    FLAT_ENGINE_API fe::lightVector2d getSize() const;
                    FLAT_ENGINE_API sf::Color getColour() const;

                    FLAT_ENGINE_API fe::sceneGraphObject *getRenderObject() const;
                    FLAT_ENGINE_API fe::rigidBody *getRigidBody() const;
                    FLAT_ENGINE_API fe::collider *getCollider() const;

                    FLAT_ENGINE_API fe::entityModules getModules() const;

                    FLAT_ENGINE_API void createModules(fe::baseGameState &currentState);

            };
    }