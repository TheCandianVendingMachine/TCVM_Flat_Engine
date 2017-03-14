#include "gameState.hpp"

void fe::gameState::removeEntity(baseEntity *entity)
    {
        m_sceneGraph.removeEntity(entity);
    }

void fe::gameState::update(float deltaTime)
    {
        m_sceneGraph.update(deltaTime);
    }

void fe::gameState::draw(sf::RenderWindow &app)
    {
        m_sceneGraph.draw(app);
    }

fe::sceneGraph *fe::gameState::getSceneGraph()
    {
        return &m_sceneGraph;
    }
