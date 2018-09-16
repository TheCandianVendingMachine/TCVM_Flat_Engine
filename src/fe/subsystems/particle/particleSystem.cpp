#include "fe/subsystems/particle/particleSystem.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include <algorithm>
#include <cmath>

void fe::particleSystem::determinePossibleCollisions(particle particle, particleNode *node)
    {
        fe::Vector2d particleAPos = m_particlePositions[particle];
        fe::circle particleABound = m_particleBounds[particle];
        // Calculate the diamter * 2 for a simple broadphase check. If the particles X position is within the 2diamter distance it is close enough to potentially collide
        float particleA2Diameter = 4.f * particleABound.m_radius;

        for (unsigned int i = node->m_data.m_firstIndex; i <= node->m_data.m_lastIndex; i++)
            {
                unsigned int particleB = m_particles[i];
                fe::Vector2d particleBPos = m_particlePositions[particleB];

                fe::Vector2d magSqr = particleBPos - particleAPos;
                if (magSqr.x < (particleA2Diameter * particleA2Diameter) &&
                    // only consider particles to the right or the bottom of the particle to avoid duplicate collision pairs
                    (particleBPos.x >= particleAPos.x || particleBPos.y >= particleAPos.y))
                    {
                        // potential collision
                        m_potentialCollisions[particle].emplace_back(particleB);
                    }
            }
    }

void fe::particleSystem::sortParticles()
    {
        std::sort(m_particles.begin(), m_particles.end(), [this](unsigned int a, unsigned int b) {
            unsigned int floorA = std::floorf(m_particlePositions[a].y / (2.f * m_particleBounds[a].m_radius));
            unsigned int floorB = std::floorf(m_particlePositions[b].y / (2.f * m_particleBounds[b].m_radius));
            if (floorA == floorB)
                {
                    // if particles have same positions the left-most particle gets closer to the top of the list
                    return m_particlePositions[a].x < m_particlePositions[b].x;
                }
            return floorA < floorB;
        });
        
        fe::particleGroup group;
        group.m_firstIndex = 0;
        unsigned int index = 0;

        for (unsigned int i = 0; i < m_particles.size(); i++)
            {   
                m_particleCollisionGroup[m_particles[i]] = index;

                if (i == m_particles.size() - 1)
                    {
                        particleNode *node = m_collisionGroups.at(index);
                        group.m_lastIndex = i;
                        if (node) 
                            {
                                node->m_data = group;
                            }
                        else 
                            {
                                m_collisionGroups.insert(group);
                            }
                        break;
                    }

                unsigned int floorA = std::floorf(m_particlePositions[i + 0].y / (2.f * m_particleBounds[i + 0].m_radius));
                unsigned int floorB = std::floorf(m_particlePositions[i + 1].y / (2.f * m_particleBounds[i + 1].m_radius));

                if (floorA != floorB)
                    {
                        group.m_lastIndex = i;
                        particleNode *node = m_collisionGroups.at(index++);
                        if (node) 
                            {
                                node->m_data = group;
                            }
                        else 
                            {
                                m_collisionGroups.insert(group);
                            }
                        group.m_firstIndex = i + 1;
                    }
            }
    }

void fe::particleSystem::broadphase()
    {
        for (unsigned int i = 0; i < m_particles.size(); i++)
            {
                particle particle = m_particles[i];
                m_potentialCollisions[particle].clear();
                m_collisionPairs[particle].clear();

                unsigned int collisionGroupIndex = m_particleCollisionGroup[particle];
                particleNode *node = m_collisionGroups.at(collisionGroupIndex);

                determinePossibleCollisions(particle, node->m_prev);
                determinePossibleCollisions(particle, node);
                determinePossibleCollisions(particle, node->m_next);
            }
    }

void fe::particleSystem::startUp()
    {
        fe::particleGroup group;
        group.m_firstIndex = 0;
        group.m_lastIndex = 0;
        m_collisionGroups.insert(group);
        m_totalParticles = 0;
        m_activeParticles = 0;
    }

void fe::particleSystem::shutDown()
    {
        m_collisionGroups.clear();
        m_groupList.clear();
    }

void fe::particleSystem::determineCollisionPairs()
    {
        sortParticles();
        broadphase();

        for (unsigned int i = 0; i < m_particles.size(); i++)
            {
                particle p0 = m_particles[i];
                fe::circle p0Bound = m_particleBounds[p0];

                for (particle j = 0; j < m_potentialCollisions[p0].size(); j++)
                    {
                        particle p1 = m_potentialCollisions[p0][j];
                        fe::circle p1Bound = m_particleBounds[p1];
                        if (fe::intersects(p0Bound, p1Bound))
                            {
                                m_collisionPairs[p0].emplace_back(p1);
                            }
                    }

            }
    }

void fe::particleSystem::fixedUpdate(float dt)
    {
    }

unsigned int fe::particleSystem::getTotalParticles() const
    {
        return m_totalParticles;
    }
