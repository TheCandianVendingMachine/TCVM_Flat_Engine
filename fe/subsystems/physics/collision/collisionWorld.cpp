#include "collisionWorld.hpp"

void fe::collisionWorld::startUp()
    {
        m_collisionBodies.startUp(FE_MAX_GAME_OBJECTS);
    }

void fe::collisionWorld::shutDown()
    {
        clear();
    }

void fe::collisionWorld::clear()
    {
        m_collisionBodies.clear();
    }

void fe::collisionWorld::handleCollisions()
    {
    }

fe::collisionBody *fe::collisionWorld::createCollider()
    {
        return m_collisionBodies.alloc();
    }

void fe::collisionWorld::deleteCollider(fe::collisionBody *body)
    {
        m_collisionBodies.free(body);
    }
