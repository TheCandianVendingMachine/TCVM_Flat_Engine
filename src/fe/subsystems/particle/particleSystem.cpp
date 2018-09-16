#include "fe/subsystems/particle/particleSystem.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/math/random.hpp"
#include "fe/engine.hpp"
#include <algorithm>
#include <cmath>

void fe::particleSystem::determinePossibleCollisions(particle particle, particleNode *node)
    {
        if (!node) return;
        fe::circle particleABound = m_particleBounds[particle];
        fe::Vector2d posA(particleABound.m_globalPositionX, particleABound.m_globalPositionY);
        // Calculate the diamter * 2 for a simple broadphase check. If the particles X position is within the 2diamter distance it is close enough to potentially collide
        float particleA2Diameter = 4.f * particleABound.m_radius;

        for (unsigned int i = node->m_data.m_firstIndex; i <= node->m_data.m_lastIndex; i++)
            {
                unsigned int particleB = m_collisionParticles[i];
                fe::circle particleBBound = m_particleBounds[particleB];
                fe::Vector2d posB(particleBBound.m_globalPositionX, particleBBound.m_globalPositionY);

                if ((posB.x - posA.x) < (particleA2Diameter * particleA2Diameter) &&
                    // only consider particles to the right or the bottom of the particle to avoid duplicate collision pairs
                    (posB.x >= posA.x || posB.y >= posA.y))
                    {
                        // potential collision
                        m_potentialCollisions[particle].emplace_back(particleB);
                    }
            }
    }

void fe::particleSystem::sortParticles()
    {
        std::sort(m_collisionParticles.begin(), m_collisionParticles.end(), [this](unsigned int a, unsigned int b) {
            int floorA = static_cast<int>(std::floorf(m_particleBounds[a].m_globalPositionY / (2.f * m_particleBounds[a].m_radius)));
            int floorB = static_cast<int>(std::floorf(m_particleBounds[a].m_globalPositionY / (2.f * m_particleBounds[b].m_radius)));
            if (floorA == floorB)
                {
                    // if particles have same positions the left-most particle gets closer to the top of the list
                    return m_particleBounds[a].m_globalPositionX < m_particleBounds[b].m_globalPositionX;
                }
            return floorA < floorB;
        });
        
        fe::particleGroup group;
        group.m_firstIndex = 0;
        unsigned int index = 0;

        for (unsigned int i = 0; i < m_collisionParticles.size(); i++)
            {   
                m_particleCollisionGroup[m_collisionParticles[i]] = index;

                if (i == m_collisionParticles.size() - 1)
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

                int floorA = static_cast<int>(std::floorf(m_particleBounds[i + 0].m_globalPositionY / (2.f * m_particleBounds[i + 0].m_radius)));
                int floorB = static_cast<int>(std::floorf(m_particleBounds[i + 1].m_globalPositionY / (2.f * m_particleBounds[i + 1].m_radius)));

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
        for (unsigned int i = 0; i < m_collisionParticles.size(); i++)
            {
                particle particle = m_collisionParticles[i];
                m_potentialCollisions[particle].clear();
                m_collisionPairs[particle].clear();
                if (m_particleFlags[particle] & fe::particleFlags::IGNORE_COLLISIONS)
                    {
                        continue;
                    }

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

        for (unsigned int i = 0; i < m_collisionParticles.size(); i++)
            {
                particle p0 = m_collisionParticles[i];
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

void fe::particleSystem::preUpdate(fe::time currentTime)
    {
        bool updateBatch = false;
        while (!m_queuedParticles.empty())
            {
                particleData p = m_queuedParticles.front();
                fe::circle bound;
                bound.m_radius = p.m_radius;
                bound.m_globalPositionX = p.m_position.x;
                bound.m_globalPositionY = p.m_position.y;
                
                m_particleVelocities.emplace_back(p.m_velocity);
                m_particleBounds.emplace_back(bound);
                m_particleFlags.emplace_back(p.m_flags);
                m_deathTime.emplace_back(p.m_deathTime);
                m_colour.emplace_back(p.m_colour);

                m_collisionParticles.emplace_back(m_totalParticles);
                m_particles.emplace_back(m_totalParticles++);
                m_queuedParticles.pop();
                updateBatch = true;
            }

        unsigned int indexDifference = 0;
        for (auto it = m_particles.begin(); it != m_particles.end();)
            {
                (*it) -= indexDifference;
                particle p = (*it);
                if (currentTime > m_deathTime[p])
                    {
                        m_particleFlags[p] = static_cast<fe::particleFlags>(m_particleFlags[p] | fe::particleFlags::KILLED);
                    }


                if (m_particleFlags[p] & fe::particleFlags::KILLED)
                    {
                        m_particleVelocities.erase(m_particleVelocities.begin() + p);
                        m_particleBounds.erase(m_particleBounds.begin() + p);
                        m_particleFlags.erase(m_particleFlags.begin() + p);
                        m_deathTime.erase(m_deathTime.begin() + p);
                        m_colour.erase(m_colour.begin() + p);

                        auto cIt = std::find(m_collisionParticles.begin(), m_collisionParticles.end(), p + indexDifference);
                        if (cIt != m_collisionParticles.end())
                            {
                                m_collisionParticles.erase(cIt);
                            }
                        else
                            {
                                FE_LOG_WARNING("Somehow a particle was made without a collision particle alongside it");
                            }

                        indexDifference++;

                        it = m_particles.erase(it);
                        m_totalParticles--;
                        updateBatch = true;
                    }
                else
                    {
                        ++it;
                    }
            }

        if (updateBatch)
            {
                m_batch.reset(m_totalParticles);
                m_particleCollisionGroup.resize(m_totalParticles);
            }
    }

void fe::particleSystem::fixedUpdate(float dt)
    {
    }

void fe::particleSystem::queueParticles(fe::time lifetime, fe::particleFlags flags, sf::Color colour, float particleRadius, unsigned int count, fe::Vector2d position, float speed, float arc, float heading)
    {
        queueParticles(lifetime, flags, colour, particleRadius, count, position, 0.f, speed, arc, heading);
    }

void fe::particleSystem::queueParticles(fe::time lifetime, fe::particleFlags flags, sf::Color colour, float particleRadius, unsigned int count, fe::Vector2d position, float radius, float speed, float arc, float heading)
    {
        float fov = arc / 2;
        for (unsigned int i = 0; i < count; i++)
            {
                particleData p;
                p.m_position = position;
                p.m_position.x += fe::random::get().generate(-radius, radius);
                p.m_position.y += fe::random::get().generate(-radius, radius);

                p.m_deathTime = fe::engine::get().getElapsedGameTime() + lifetime;
                p.m_radius = particleRadius;
                p.m_flags = flags;
                p.m_colour = colour;

                p.m_velocity.x = speed * std::cos(heading + fe::random::get().generate(-fov, fov));
                p.m_velocity.y = speed * std::sin(heading + fe::random::get().generate(-fov, fov));

                m_queuedParticles.push(p);
            }
    }

void fe::particleSystem::draw(sf::RenderTarget &target)
    {
        for (particle i = 0; i < m_totalParticles; i++)
            {
                m_batch.update(i, m_particleBounds[i].m_globalPositionX, m_particleBounds[i].m_globalPositionY, m_particleBounds[i].m_radius, m_colour[i]);
            }
        m_batch.draw(target);
    }

unsigned int fe::particleSystem::getTotalParticles() const
    {
        return m_totalParticles;
    }
