#include "fe/subsystems/particle/particleSystem.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/math/random.hpp"
#include "fe/engine.hpp"
#include "fe/debug/profiler.hpp"
#include <algorithm>
#include <cmath>

void fe::particleSystem::sortParticles(const fe::circle *boundsData, const unsigned int *collisionParticlesData, const int *floorData)
    {
        std::sort(m_collisionParticles.begin(), m_collisionParticles.end(), [boundsData, floorData](unsigned int a, unsigned int b) {
            if (floorData[a] == floorData[b])
                {
                    // if particles have same positions the left-most particle gets closer to the top of the list
                    return boundsData[a].m_globalPositionX < boundsData[b].m_globalPositionX;
                }
            return floorData[a] < floorData[b];
        });
        
        fe::particleGroup group;
        group.m_firstIndex = 0;
        unsigned int index = 0;
        for (unsigned int i = 0; i < m_collisionParticles.size() - 1; i++)
            {
                if (floorData[collisionParticlesData[i + 0]] != floorData[collisionParticlesData[i + 1]])
                    {
                        group.m_lastIndex = i;
                        if (index < m_collisionGroups.size())
                            {
                                m_collisionGroups.data()[index++] = group;
                            }
                        else 
                            {
                                m_collisionGroups.emplace_back(group);
                            }
                        group.m_firstIndex = i + 1;
                    }
            }

        group.m_lastIndex = m_collisionParticles.size() - 1;
        m_collisionGroups.data()[index] = group;
    }

void fe::particleSystem::broadphase(const unsigned int *collisionParticlesData, const fe::circle *boundsData)
    {
        fe::particleGroup *collisionGroupData = m_collisionGroups.data();
        for (unsigned int i = 0; i < m_collisionGroups.size(); i++)
            {
                fe::particleGroup &node = m_collisionGroups.data()[i];
                // Since this data is contigious in memory we can guarentee that a constant iteration will not fuck up
                unsigned int firstIndex = node.m_firstIndex;
                unsigned int lastIndex = node.m_lastIndex;

                if (i > 0)
                    {
                        firstIndex = collisionGroupData[i - 1].m_firstIndex;
                    }
                if (i < m_collisionGroups.size() - 1)
                    {
                        lastIndex = collisionGroupData[i + 1].m_lastIndex;
                    }

                for (unsigned int x = node.m_firstIndex; x <= node.m_lastIndex; x++)
                    {
                        particle particle = collisionParticlesData[x];
                        if (!(m_particleFlags.data()[particle] & fe::particleFlags::IGNORE_COLLISIONS))
                            {
                                if (m_collisionPairIndex < m_collisionPairs.size())
                                    {
                                        m_collisionPairs.data()[m_collisionPairIndex] = particle;
                                    }
                                else 
                                    {
                                        m_collisionPairs.emplace_back(particle);
                                    }
                                m_collisionPairs.data()[m_collisionPairIndex++].m_collider = true;

                                float posAX = boundsData[particle].m_globalPositionX;
                                float posAY = boundsData[particle].m_globalPositionY;
                                // Calculate the diamter * 2 for a simple broadphase check. If the particles X position is within the 2diamter distance it is close enough to potentially collide
                                float particleA2Diameter = 4.f * boundsData[particle].m_radius;

                                for (unsigned int y = firstIndex; y <= lastIndex; y++)
                                    {
                                        unsigned int particleB = collisionParticlesData[y];
                                        float posBX = boundsData[particleB].m_globalPositionX;
                                        float posBY = boundsData[particleB].m_globalPositionY;

                                        if (std::abs(static_cast<int>(posBX - posAX)) < particleA2Diameter &&
                                            // only consider particles to the right or the bottom of the particle to avoid duplicate collision pairs
                                            (posBX > posAX || posBY > posAY))
                                            {
                                                // potential collision
                                                if (m_collisionPairIndex < m_collisionPairs.size())
                                                    {
                                                        m_collisionPairs.data()[m_collisionPairIndex] = particleB;
                                                    }
                                                else 
                                                    {
                                                        m_collisionPairs.emplace_back(particleB);
                                                    }
                                                m_collisionPairIndex++;
                                            }
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
        m_collisionGroups.emplace_back(group);
        m_totalParticles = 0;

        m_collisionPairIndex = 0;
        m_collisionPairEndIndex = 0;
    }

void fe::particleSystem::shutDown()
    {
        m_collisionGroups.clear();
        m_groupList.clear();
    }

void fe::particleSystem::determineCollisionPairs()
    {
        FE_ENGINE_PROFILE("particle", "determine_collision_pairs");
        const unsigned int *collisionParticlesData = m_collisionParticles.data();
        const fe::circle *boundsData = m_particleBounds.data();

        FE_ENGINE_PROFILE("particle", "sort_particles");
        sortParticles(boundsData, collisionParticlesData, m_particleFloor.data());
        FE_END_PROFILE;
        FE_ENGINE_PROFILE("particle", "broadphase");
        broadphase(collisionParticlesData, boundsData);
        FE_END_PROFILE;

        m_collisionPairEndIndex = std::max(m_collisionPairEndIndex, m_collisionPairIndex);

        FE_ENGINE_PROFILE("particle", "cull_pairs");
        particle itOrigParticle = 0;
        for (auto it = m_collisionPairs.begin(); it != (m_collisionPairs.begin() + m_collisionPairEndIndex); ++it)
            {
                if (it->m_collider)
                    {
                        itOrigParticle = it->m_particle;
                    }
                else if (!fe::intersects(boundsData[itOrigParticle], boundsData[it->m_particle]))
                    {
                        it->m_collided = false;
                    }
            }

        FE_END_PROFILE;
        m_collisionPairIndex = 0;
        FE_END_PROFILE;
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
                m_particleFloor.emplace_back(static_cast<int>(std::floorf(m_particleBounds.data()[m_totalParticles].m_globalPositionY / (2.f * m_particleBounds.data()[m_totalParticles].m_radius))));

                m_collisionParticles.emplace_back(m_totalParticles++);
                m_queuedParticles.pop();
                updateBatch = true;
            }

        unsigned int indexDifference = 0;
        for (unsigned int i = 0; i < m_totalParticles;)
            {
                particle p = i - indexDifference;
                if (currentTime > m_deathTime[p])
                    {
                        m_particleFlags.data()[p] = static_cast<fe::particleFlags>(m_particleFlags.data()[p] | fe::particleFlags::KILLED);
                    }


                if (m_particleFlags.data()[p] & fe::particleFlags::KILLED)
                    {
                        m_particleVelocities.erase(m_particleVelocities.begin() + p);
                        m_particleBounds.erase(m_particleBounds.begin() + p);
                        m_particleFlags.erase(m_particleFlags.begin() + p);
                        m_deathTime.erase(m_deathTime.begin() + p);
                        m_colour.erase(m_colour.begin() + p);
                        m_particleFloor.erase(m_particleFloor.begin() + p);

                        auto cIt = std::find(m_collisionParticles.begin(), m_collisionParticles.end(), p + indexDifference);
                        m_collisionParticles.erase(cIt);
                        indexDifference++;

                        m_totalParticles--;
                        updateBatch = true;
                    }
                else
                    {
                        ++i;
                    }
            }

        if (updateBatch)
            {
                m_batch.reset(m_totalParticles);
            }
    }

void fe::particleSystem::fixedUpdate(float dt)
    {
        /*for (unsigned int i = 0; i < m_totalParticles; i++)
            {
                m_particleFloor.data()[i] = static_cast<int>(std::floorf(m_particleBounds.data()[i].m_globalPositionY / (2.f * m_particleBounds.data()[i].m_radius)));
            }*/
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
