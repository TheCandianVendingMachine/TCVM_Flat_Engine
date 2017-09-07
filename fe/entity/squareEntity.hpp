// squareEntity.hpp
// A basic class for a basic square. Made so we dont need to create a square class for every square in a game
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"

#include "baseEntity.hpp"

namespace fe
    {
        class squareEntity : public fe::baseEntity
            {
                public:
                    FLAT_ENGINE_API squareEntity(entityModules modules = fe::entityModules::RENDER_OBJECT, bool staticObject = false);
                    FLAT_ENGINE_API squareEntity(fe::Vector2d size, entityModules modules = fe::entityModules::RENDER_OBJECT, bool staticObject = false);
                    FLAT_ENGINE_API squareEntity(sf::Color colour, entityModules modules = fe::entityModules::RENDER_OBJECT, bool staticObject = false);
                    FLAT_ENGINE_API squareEntity(fe::Vector2d size, sf::Color colour, entityModules modules = fe::entityModules::RENDER_OBJECT, bool staticObject = false);
            };
    }