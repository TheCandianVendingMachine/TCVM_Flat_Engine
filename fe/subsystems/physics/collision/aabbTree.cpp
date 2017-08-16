#include "aabbTree.hpp"
#include "collisionBody.hpp"

#include "../../../debug/debugDraw.hpp"
#include "../../../debug/profiler.hpp"

void fe::aabbTree::debugDrawAABB(int node)
    {
        return void();
    }

void fe::aabbTree::updateAABB(int node)
    {
        return void();
    }

void fe::aabbTree::insert(int node, int parent)
    {
        return void();
    }

void fe::aabbTree::balance(int node)
    {
        return void();
    }

fe::aabbTree::aabbTree()
    {
    }

void fe::aabbTree::startUp()
    {
        return void();
    }

void fe::aabbTree::add(fe::collider *collider)
    {
        return void();
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
        return void();
    }

void fe::aabbTree::update()
    {
        return void();
    }

const std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int> fe::aabbTree::computeColliderPairs()
    {
        return std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int>();
    }

fe::collider *fe::aabbTree::colliderAtPoint(float x, float y)
    {
        return nullptr;
    }

std::pair<fe::collider*[FE_MAX_GAME_OBJECTS], unsigned int> fe::aabbTree::collidesWithAABB(fe::collider &aabb)
    {
        return std::pair<fe::collider*[FE_MAX_GAME_OBJECTS], unsigned int>();
    }

fe::raycastResult fe::aabbTree::raycast(float x, float y, float dirX, float dirY)
    {
        return fe::raycastResult();
    }