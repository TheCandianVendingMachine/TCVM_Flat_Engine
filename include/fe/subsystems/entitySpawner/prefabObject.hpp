// prefabObject.hpp
// entity prefab object
#pragma once
#include "fe/flatEngineExport.hpp"
#include <unordered_map>
#include <vector>
#include "fe/objectManagement/str.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/subsystems/resourceManager/fontData.hpp"
#include "fe/entity/entityModules.hpp"
#include "fe/subsystems/scripting/luaFunctionReference.hpp"
#include <SFML/Graphics/Color.hpp>

namespace fe
    {
        struct prefabObject
            {
                fe::luaFunctionReference *m_onAdd = nullptr;
                fe::luaFunctionReference *m_onRemove = nullptr;
                fe::luaFunctionReference *m_update = nullptr;
                fe::luaFunctionReference *m_postUpdate = nullptr;
                fe::luaFunctionReference *m_fixedUpdate = nullptr;
                fe::luaFunctionReference *m_onCollision = nullptr;
                sol::table m_entityTable;

                std::unordered_map<fe::str, fe::luaFunctionReference*> m_userFunctions;
                std::unordered_map<fe::str, fe::luaFunctionReference*> m_events;

                std::vector<std::pair<std::string, sol::table>> m_components;

                fe::str m_tag = 0;
                fe::fontData m_font = fe::fontData();
                sf::Color m_colour;
                fe::lightVector2<unsigned int> m_size;
                fe::lightVector2<unsigned int> m_colliderSize;
                fe::lightVector2d m_colliderPosition;
                fe::lightVector2<unsigned int> m_textureOffset;
                fe::lightVector2<unsigned int> m_textureSize;
                fe::Handle m_animationHandle;
                fe::str m_collisionEvent = 0;
                fe::str m_textureID = 0;
                fe::str m_collisionGroup = 0;
                int m_connected = -1;
                int m_zPosition = 0;
                float m_maxSpeed = 0.f;
                float m_mass = 1.f;
                float m_frictionCoef = 0.f;
                float m_completionRadius = 0.f;
                float m_slowdownDistance = 0.f;
                fe::entityModules m_modules = fe::entityModules::NONE;
                bool m_solid = true;

                FLAT_ENGINE_API prefabObject &addUserFunction(const char *luaName, const char *funcName);
            };
    }
