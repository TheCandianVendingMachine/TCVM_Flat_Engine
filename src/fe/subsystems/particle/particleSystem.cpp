#include "fe/subsystems/particle/particleSystem.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/math/random.hpp"
#include "fe/engine.hpp"
#include <algorithm>
#include <cmath>

void fe::particleSystem::determinePossibleCollisions(particle particle, particleNode *node, const fe::circle *boundsData, const unsigned int *collisionParticlesData)
    {
        if (!node) return;
        float posAX = boundsData[particle].m_globalPositionX;
        float posAY = boundsData[particle].m_globalPositionY;
        // Calculate the diamter * 2 for a simple broadphase check. If the particles X position is within the 2diamter distance it is close enough to potentially collide
        float particleA2Diameter = 4.f * boundsData[particle].m_radius;

        unsigned int index = m_collisionPairs.size();
        m_collisionPairs.emplace_back(particle);
        m_collisionPairs.back().m_collider = true;

        for (unsigned int i = node->m_data.m_firstIndex; i <= node->m_data.m_lastIndex; i++)
            {
                unsigned int particleB = collisionParticlesData[i];
                float posBX = boundsData[particleB].m_globalPositionX;
                float posBY = boundsData[particleB].m_globalPositionY;

                if ((posBX - posAX) < (particleA2Diameter * particleA2Diameter) &&
                    // only consider particles to the right or the bottom of the particle to avoid duplicate collision pairs
                    (posBX >= posAX || posBY >= posAY))
                    {
                        // potential collision
                        m_collisionPairs.emplace_back(particleB);
                    }
            }

        (m_collisionPairs.begin() + index)->m_size = m_collisionPairs.size() - index;
    }

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
        for (unsigned int i = 0; i < m_collisionParticles.size(); i++)
            {
                particle particle = collisionParticlesData[i];
                m_collisionPairs.clear();
                if (m_particleFlags[particle] & fe::particleFlags::IGNORE_COLLISIONS)
                    {
                        continue;
                    }

                unsigned int collisionGroupIndex = collisionGroupData[particle];
                particleNode *node = m_collisionGroups.at(collisionGroupIndex);

                determinePossibleCollisions(particle, node->m_prev, boundsData, collisionParticlesData);
                determinePossibleCollisions(particle, node, boundsData, collisionParticlesData);
                determinePossibleCollisions(particle, node->m_next, boundsData, collisionParticlesData);
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
        const unsigned int *collisionParticlesData = m_collisionParticles.data();
        const int *collisionGroupData = m_particleCollisionGroup.data();
        const fe::circle *boundsData = m_particleBounds.data();

        sortParticles(boundsData, collisionParticlesData);
        broadphase(collisionParticlesData, collisionGroupData, boundsData);

        for (unsigned int i = 0; i < m_collisionParticles.size(); i++)
            {
                particle p0 = collisionParticlesData[i];

                auto it = std::find_if(m_collisionPairs.begin(), m_collisionPairs.end(), [p0](particleCollisionData &p) { return p.m_collider && p.m_particle == p0; });

                if (it != m_collisionPairs.end())
                    {
                        for (unsigned int i = it->m_size; i > 0; --i)
                            {
                                it++;
                                particle p1 = (*it).m_particle;
                                fe::circle p0Bound = boundsData[p0];
                                fe::circle p1Bound = boundsData[p1];
                                if (fe::intersects(p0Bound, p1Bound))
                                    {
                                        m_collisionPairs.emplace_back(p1);
                                    }
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
