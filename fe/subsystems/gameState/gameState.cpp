#include "gameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::baseGameState::removeEntity(baseEntity *entity)
    {
        entity->onRemove(*this);
        m_sceneGraph.removeEntity(entity);
    }

void fe::baseGameState::update(float deltaTime)
    {
        m_sceneGraph.update(deltaTime);
    }

void fe::baseGameState::postUpdateDefined()
    {
        m_sceneGraph.postUpdate();
        m_collisionHandler.handleCollisions();
    }

void fe::baseGameState::draw(sf::RenderTarget &app)
    {
        m_sceneGraph.draw(app);
    }

fe::sceneGraph &fe::baseGameState::getSceneGraph()
    {
        return m_sceneGraph;
    }

fe::collisonHandler &fe::baseGameState::getCollisionHandler()
    {
        return m_collisionHandler;
    }
