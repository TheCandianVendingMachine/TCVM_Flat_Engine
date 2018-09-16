#include "fe/subsystems/particle/particleSystem.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/math/random.hpp"
#include "fe/engine.hpp"
#include <algorithm>
#include <cmath>

void fe::particleSystem::sortParticles(const fe::circle *boundsData, const unsigned int *collisionParticlesData)
    {
        std::sort(m_collisionParticles.begin(), m_collisionParticles.end(), [boundsData](unsigned int a, unsigned int b) {
            int floorA = static_cast<int>(std::floorf(boundsData[a].m_globalPositionY / (2.f * boundsData[a].m_radius)));
            int floorB = static_cast<int>(std::floorf(boundsData[b].m_globalPositionY / (2.f * boundsData[b].m_radius)));
            if (floorA == floorB)
                {
                    // if particles have same positions the left-most particle gets closer to the top of the list
                    return boundsData[a].m_globalPositionX < boundsData[b].m_globalPositionX;
                }
            return floorA < floorB;
        });
        
        fe::particleGroup group;
        group.m_firstIndex = 0;
        unsigned int index = 0;
        for (unsigned int i = 0; i < m_collisionParticles.size(); i++)
            {   
                m_particleCollisionGroup[collisionParticlesData[i]] = index;

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

                int floorA = static_cast<int>(std::floorf(boundsData[collisionParticlesData[i + 0]].m_globalPositionY / (2.f * boundsData[collisionParticlesData[i + 0]].m_radius)));
                int floorB = static_cast<int>(std::floorf(boundsData[collisionParticlesData[i + 1]].m_globalPositionY / (2.f * boundsData[collisionParticlesData[i + 1]].m_radius)));
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

void fe::particleSystem::broadphase(const unsigned int *collisionParticlesData, const int *collisionGroupData, const fe::circle *boundsData)
    {
        for (unsigned int i = 0; i < m_collisionGroups.size(); i++)
            {
                particleNode *node = m_collisionGroups.at(i);
                for (unsigned int x = node->m_data.m_firstIndex; x <= node->m_data.m_lastIndex; x++)
                    {
                        particle particle = collisionParticlesData[x];
                        if (m_particleFlags.data()[particle] & fe::particleFlags::IGNORE_COLLISIONS)
                            {
                                continue;
                            }

                        m_collisionPairs.emplace_back(particle);
                        m_collisionPairs.back().m_collider = true;

                        float posAX = boundsData[particle].m_globalPositionX;
                        float posAY = boundsData[particle].m_globalPositionY;
                        // Calculate the diamter * 2 for a simple broadphase check. If the particles X position is within the 2diamter distance it is close enough to potentially collide
                        float particleA2Diameter = 4.f * boundsData[particle].m_radius;

                        // Since this data is contigious in memory we can guarentee that a constant iteration will not fuck up
                        unsigned int firstIndex = node->m_data.m_firstIndex;
                        unsigned int lastIndex = node->m_data.m_lastIndex;

                        if (node->m_prev)
                            {
                                firstIndex = node->m_prev->m_data.m_firstIndex;
                            }
                        if (node->m_next)
                            {
                                lastIndex = node->m_next->m_data.m_lastIndex;
                            }

                        for (unsigned int i = firstIndex; i <= lastIndex; i++)
                            {
                                unsigned int particleB = collisionParticlesData[i];
                                float posBX = boundsData[particleB].m_globalPositionX;
                                float posBY = boundsData[particleB].m_globalPositionY;

                                if (std::abs(posBX - posAX) < particleA2Diameter &&
                                    // only consider particles to the right or the bottom of the particle to avoid duplicate collision pairs
                                    (posBX > posAX || posBY > posAY))
                                    {
                                        // potential collision
                                        m_collisionPairs.emplace_back(particleB);
                                    }
                            }
                    }
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
        m_collisionPairs.clear();
        const unsigned int *collisionParticlesData = m_collisionParticles.data();
        const int *collisionGroupData = m_particleCollisionGroup.data();
        const fe::circle *boundsData = m_particleBounds.data();

        sortParticles(boundsData, collisionParticlesData);
        broadphase(collisionParticlesData, collisionGroupData, boundsData);

        auto itOrig = m_collisionPairs.begin();
        particle itOrigParticle = itOrig->m_particle;
        for (auto it = itOrig + 1; it != m_collisionPairs.end();)
            {
                if (it->m_collider)
                    {
                        itOrig = it;
                        itOrigParticle = itOrig->m_particle;
                    }

                if (!fe::intersects(boundsData[itOrigParticle], boundsData[it->m_particle]))
                    {
                        it = m_collisionPairs.erase(it);
                    }
                else
                    {
                        ++it;
                    }
            }

        for (auto it = m_collisionPairs.begin(); (it + 1) != m_collisionPairs.end();)
            {
                if (it->m_collider && (it + 1)->m_collider)
                    {
                        it = m_collisionPairs.erase(it);
                    }
                else
                    {
                        ++it;
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
                        m_collisionParticles.erase(cIt);
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
                m_batch.update(i, m_particleBounds.data()[i].m_globalPositionX, m_particleBounds.data()[i].m_globalPositionY, m_particleBounds.data()[i].m_radius, m_colour.data()[i]);
            }
        m_batch.draw(target);
    }

unsigned int fe::particleSystem::getTotalParticles() const
    {
        return m_totalParticles;
    }
