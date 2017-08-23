// squareEntity.hpp
// A basic class for a basic square. Made so we dont need to create a square class for every square in a game
#pragma once
#include "baseEntity.hpp"

namespace fe
    {
        class squareEntity : public fe::baseEntity
            {
                public:
                    squareEntity(entityModules modules = fe::entityModules::RENDER_OBJECT);
                    squareEntity(fe::Vector2d size, entityModules modules = fe::entityModules::RENDER_OBJECT);
                    squareEntity(sf::Color colour, entityModules modules = fe::entityModules::RENDER_OBJECT);
                    squareEntity(fe::Vector2d size, sf::Color colour, entityModules modules = fe::entityModules::RENDER_OBJECT);
            };
    }