#include "gameState.hpp"

void fe::baseGameState::removeEntity(baseEntity *entity)
    {
        m_sceneGraph.removeEntity(entity);
    }

void fe::baseGameState::update(float deltaTime)
    {
        m_sceneGraph.update(deltaTime);
    }

void fe::baseGameState::draw(sf::RenderWindow &app)
    {
        m_sceneGraph.draw(app);
    }

fe::sceneGraph *fe::baseGameState::getSceneGraph()
    {
        return &m_sceneGraph;
    }
