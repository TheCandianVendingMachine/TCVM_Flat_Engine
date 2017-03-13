#include "gameState.hpp"

void fe::gameState::init()
    {
    }

void fe::gameState::preUpdate()
    {
    }

void fe::gameState::update(float deltaTime)
    {
    }

void fe::gameState::postUpdate()
    {
    }

void fe::gameState::preDraw()
    {
    }

void fe::gameState::draw(sf::RenderWindow &app)
    {
    }

void fe::gameState::postDraw()
    {
    }

void fe::gameState::deinit()
    {
    }

fe::sceneGraph *fe::gameState::getSceneGraph()
    {
        return &m_sceneGraph;
    }
