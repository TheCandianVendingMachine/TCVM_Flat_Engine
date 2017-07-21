#include "aabbTree.hpp"
#include "collisionBody.hpp"

#include "../../../debug/debugDraw.hpp"
#include "../../../debug/profiler.hpp"

void fe::aabbTree::debugDrawAABB(node *base)
    {
        if (!base) return;
        debugDrawAABB(base->m_leftChild);
        debugDrawAABB(base->m_rightChild);

        FE_DEBUG_DRAW_SQUARE(base->m_fatAABB.m_sizeX, base->m_fatAABB.m_sizeY, base->m_fatAABB.m_positionX, base->m_fatAABB.m_positionY, sf::Color::Red);
    }

void fe::aabbTree::updateAABB(node *baseNode)
    {
        if (!baseNode) return;
        if (baseNode->isLeaf())
            {
                fe::collider *collider = baseNode->m_userData;
                baseNode->m_fatAABB.m_positionX = collider->m_aabb.m_positionX - m_fatness;
                baseNode->m_fatAABB.m_positionY = collider->m_aabb.m_positionY - m_fatness;
                baseNode->m_fatAABB.m_sizeX = collider->m_aabb.m_sizeX + m_fatness;
                baseNode->m_fatAABB.m_sizeY = collider->m_aabb.m_sizeY + m_fatness;
            }
        else
            {
                updateAABB(baseNode->m_leftChild);
                updateAABB(baseNode->m_rightChild);

                float minX = std::min(baseNode->m_leftChild->m_fatAABB.m_positionX, baseNode->m_rightChild->m_fatAABB.m_positionX);
                float minY = std::min(baseNode->m_leftChild->m_fatAABB.m_positionY, baseNode->m_rightChild->m_fatAABB.m_positionY);
                float maxX = std::max(baseNode->m_leftChild->m_fatAABB.m_positionX, baseNode->m_rightChild->m_fatAABB.m_positionX);
                float maxY = std::max(baseNode->m_leftChild->m_fatAABB.m_positionY, baseNode->m_rightChild->m_fatAABB.m_positionY);

                float leftSizeX = baseNode->m_leftChild->m_fatAABB.m_positionX < baseNode->m_rightChild->m_fatAABB.m_sizeX ? baseNode->m_leftChild->m_fatAABB.m_sizeX : baseNode->m_rightChild->m_fatAABB.m_sizeX;
                float leftSizeY = baseNode->m_leftChild->m_fatAABB.m_positionY < baseNode->m_rightChild->m_fatAABB.m_sizeY ? baseNode->m_leftChild->m_fatAABB.m_sizeY : baseNode->m_rightChild->m_fatAABB.m_sizeY;

                baseNode->m_fatAABB.m_positionX = minX - m_fatness;
                baseNode->m_fatAABB.m_positionY = minY - m_fatness;
                baseNode->m_fatAABB.m_sizeX = baseNode->m_leftChild->m_fatAABB.m_sizeX + baseNode->m_rightChild->m_fatAABB.m_sizeX + (maxX - (minX + leftSizeX)) + m_fatness;
                baseNode->m_fatAABB.m_sizeY = baseNode->m_leftChild->m_fatAABB.m_sizeY + baseNode->m_rightChild->m_fatAABB.m_sizeY + (maxY - (minY + leftSizeY)) + m_fatness;
            }
    }

void fe::aabbTree::insert(node *baseNode, node **parent)
    {
        if ((*parent)->isLeaf())
            {
                node *newNode = m_nodes.alloc();
                newNode->m_userData = (*parent)->m_userData;
                (*parent)->m_userData = nullptr;

                (*parent)->m_leftChild = newNode;
                (*parent)->m_rightChild = baseNode;

                newNode->m_parent = (*parent);
                baseNode->m_parent = (*parent);
            }
        else
            {
                insert(baseNode, &((*parent)->m_leftChild->m_fatAABB.volume() < (*parent)->m_rightChild->m_fatAABB.volume() ? (*parent)->m_leftChild : (*parent)->m_rightChild));
            }

        updateAABB((*parent));
    }


fe::aabbTree::aabbTree() : m_fatness(5.f), m_base(nullptr)
    {
    }

void fe::aabbTree::startUp()
    {
        m_nodes.startUp(FE_MAX_GAME_OBJECTS);
    }

void fe::aabbTree::add(fe::collider *collider)
    {
        if (m_base)
            {
                node *newNode = m_nodes.alloc();
                newNode->m_userData = collider;
                insert(newNode, &m_base);
            }
        else
            {
                m_base = m_nodes.alloc();
                m_base->m_userData = collider;
            }
    }

void fe::aabbTree::remove(fe::collider *collider)
    {
        
    }

void fe::aabbTree::update(float dt)
    {
        updateAABB(m_base);
        if (m_debug)
            {
                debugDrawAABB(m_base);
            }
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
