#include "squareEntity.hpp"

fe::squareEntity::squareEntity(entityModules modules) : fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT)
    {
    }

fe::squareEntity::squareEntity(fe::Vector2d size, entityModules modules) : fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT)
    {
        setSize(size);
    }

fe::squareEntity::squareEntity(sf::Color colour, entityModules modules) : fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT)
    {
        setColour(colour);
    }

fe::squareEntity::squareEntity(fe::Vector2d size, sf::Color colour, entityModules modules) : fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT)
    {
        setSize(size);
        setColour(colour);
    }
