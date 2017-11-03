#include "squareEntity.hpp"

fe::squareEntity::squareEntity(entityModules modules, bool staticObject) : 
    fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT, staticObject)
    {
    }

fe::squareEntity::squareEntity(fe::Vector2d size, entityModules modules, bool staticObject) : 
    fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT, staticObject)
    {
        setSize(size);
    }

fe::squareEntity::squareEntity(sf::Color colour, entityModules modules, bool staticObject) : 
    fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT, staticObject)
    {
        setColour(colour);
    }

fe::squareEntity::squareEntity(fe::Vector2d size, sf::Color colour, entityModules modules, bool staticObject) : 
    fe::baseEntity(modules | fe::entityModules::RENDER_OBJECT, staticObject)
    {
        setSize(size);
        setColour(colour);
    }
