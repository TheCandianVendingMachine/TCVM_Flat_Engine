#include "fe/subsystems/particle/particleSystem.hpp"
#include "fe/subsystems/collision/bounds/circleTests.hpp"
#include "fe/math/random.hpp"
#include "fe/engine.hpp"
#include "fe/debug/profiler.hpp"
#include <algorithm>
#include <cmath>

void fe::particleSystem::sortParticles(const fe::circle *boundsData, const unsigned int *collisionParticlesData, const int *floorData)
    {
        if (m_collisionParticles.size() == 0) return;
        FE_ENGINE_PROFILE("particle_sort", "inplace_sort");
        std::sort(m_collisionParticles.begin(), m_collisionParticles.end(), [boundsData, floorData](unsigned int a, unsigned int b) {
            return floorData[a] < floorData[b] || (floorData[a] == floorData[b] && boundsData[a].m_globalPositionX < boundsData[b].m_globalPositionX);
        });
        FE_END_PROFILE;
        
        FE_ENGINE_PROFILE("particle_sort", "determine_groups");
        fe::particleGroup group;
        group.m_firstIndex = 0;
        for (unsigned int i = 0; i < m_collisionParticles.size() - 1; i++)
            {
                if (floorData[collisionParticlesData[i + 0]] != floorData[collisionParticlesData[i + 1]])
                    {
                        group.m_lastIndex = i;
                        m_collisionGroups.emplace_back(group);
                        group.m_firstIndex = i + 1;
                    }
            }

        group.m_lastIndex = m_collisionParticles.size() - 1;
        m_collisionGroups.emplace_back(group);
        FE_END_PROFILE;
    }

void fe::particleSystem::broadphase(const unsigned int *collisionParticlesData, const fe::circle *boundsData)
    {
        fe::particleGroup *collisionGroupData = m_collisionGroups.data();
        for (unsigned int i = 0; i < m_collisionGroups.size(); i++)
            {
                fe::particleGroup &node = collisionGroupData[i];
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

                                // casting to an int has substantial performance improvements with no optimizations however
                                // when optimizations are enabled performance is hindered by this casting
                            #if _DEBUG
                                if (std::abs(static_cast<int>(posBX - posAX)) < particleA2Diameter &&
                            #else
                                if (std::abs(posBX - posAX) < particleA2Diameter &&
                            #endif
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

void fe::particleSystem::startUp()
    {
        m_totalParticles = 0;
        m_collisionPairIndex = 0;
        m_repulsionCoefficient = 1.f;
        m_weightAverage = 0.1f;
        m_particleWeight = 5.f;
    }

void fe::particleSystem::shutDown()
    {
        m_collisionGroups.clear();
        m_groupList.clear();
    }

void fe::particleSystem::determineCollisionPairs()
    {
        FE_ENGINE_PROFILE("particle", "determine_collision_pairs");
        m_collisionGroups.clear();
        const unsigned int *collisionParticlesData = m_collisionParticles.data();
        const fe::circle *boundsData = m_particleBounds.data();

        FE_ENGINE_PROFILE("particle", "sort_particles");
        sortParticles(boundsData, collisionParticlesData, m_particleFloor.data());
        FE_END_PROFILE;
        FE_ENGINE_PROFILE("particle", "broadphase");
        broadphase(collisionParticlesData, boundsData);
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("particle", "cull_pairs");
        particle itOrigParticle = 0;
        for (unsigned int i = 0; i < m_collisionPairIndex; i++)
            {
                particleCollisionData &data = m_collisionPairs.data()[i];
                if (data.m_collider)
                    {
                        itOrigParticle = data.m_particle;
                    }
                else if (!fe::intersects(boundsData[itOrigParticle], boundsData[data.m_particle]))
                    {
                        data.m_collided = false;
                    }
                else
                    {
                        float diffX = boundsData[data.m_particle].m_globalPositionX - boundsData[itOrigParticle].m_globalPositionX;
                        float diffY = boundsData[data.m_particle].m_globalPositionY - boundsData[itOrigParticle].m_globalPositionY;
                        float diamter = boundsData[data.m_particle].m_radius + boundsData[itOrigParticle].m_radius;
                        float distance = std::sqrt(diffX * diffX + diffY * diffY);

                        data.m_relativePositionX = diffX / distance;
                        data.m_relativePositionY = diffY / distance;
                        data.m_overlap = 1 - (distance / diamter);
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

                if (!(p.m_flags & fe::particleFlags::IGNORE_COLLISIONS))
                    {
                        m_collisionParticles.emplace_back(m_totalParticles);
                    }
                m_totalParticles++;
                m_queuedParticles.pop();
                updateBatch = true;
            }

        for (unsigned int i = 0; i < m_totalParticles; ++i)
            {
                if (currentTime > m_deathTime[i])
                    {
                        m_particleFlags.data()[i] = static_cast<fe::particleFlags>(m_particleFlags.data()[i] | fe::particleFlags::KILLED);
                    }
            }

        FE_ENGINE_PROFILE("particle_deletion", "killed");
        for (int i = (m_totalParticles - 1); i >= 0; i--)
            {
                if (m_particleFlags.data()[i] & fe::particleFlags::KILLED)
                    {
                        m_particleVelocities.erase(m_particleVelocities.begin() + i);
                        m_particleBounds.erase(m_particleBounds.begin() + i);
                        m_particleFlags.erase(m_particleFlags.begin() + i);
                        m_deathTime.erase(m_deathTime.begin() + i);
                        m_colour.erase(m_colour.begin() + i);
                        m_particleFloor.erase(m_particleFloor.begin() + i);

                        m_collisionParticles.erase(std::remove(m_collisionParticles.begin(), m_collisionParticles.end(), i), m_collisionParticles.end());

                        m_totalParticles--;
                        updateBatch = true;
                    }
            }
        FE_END_PROFILE;

        if (updateBatch)
            {
                m_batch.reset(m_totalParticles);
            }
    }

void fe::particleSystem::update()
    {
        FE_ENGINE_PROFILE("particle_sim", "collision_pair_solutions");
        particleCollisionData *collisionPairData = m_collisionPairs.data();
        for (unsigned int i = 0; i < m_collisionPairs.size(); i++)
            {
                int j = i;
                float weightSum = 0.f;
                while (!collisionPairData[i].m_collider)
                    {
                        weightSum += collisionPairData[i].m_overlap * static_cast<int>(collisionPairData[i].m_collided);
                        i++;
                    }
                collisionPairData[j].m_pressure = std::max(0.f, m_particleWeight * (weightSum - m_weightAverage));
            }
        FE_END_PROFILE;
    }

void fe::particleSystem::fixedUpdate(float dt)
    {
        fe::circle *boundsData = m_particleBounds.data();
        fe::Vector2d *velocityData = m_particleVelocities.data();
        int *particleFloorData = m_particleFloor.data();

        const particleCollisionData *collisionPairData = m_collisionPairs.data();

        FE_ENGINE_PROFILE("particle_sim", "apply_pressure");
        for (unsigned int i = 0; i < m_collisionPairs.size(); i++)
            {
                int j = i;
                particle p = collisionPairData[j].m_particle;
                fe::Vector2d velocity = velocityData[p];
                while (!collisionPairData[i].m_collider)
                    {
                        fe::Vector2d relativeOverlap(collisionPairData[i].m_relativePositionX, collisionPairData[i].m_relativePositionY);
                        velocity = velocity + dt * m_repulsionCoefficient * (collisionPairData[i].m_pressure + collisionPairData[j].m_pressure) * collisionPairData[i].m_overlap * relativeOverlap;
                        i++;
                    }
                velocityData[p] = velocity;
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("particle_sim", "velocity_update");
        for (unsigned int i = 0; i < m_totalParticles; i++)
            {
                boundsData[i].m_globalPositionX += dt * velocityData[i].x;
                boundsData[i].m_globalPositionY += dt * velocityData[i].y;

                particleFloorData[i] = static_cast<int>(std::floorf(boundsData[i].m_globalPositionY / (2.f * boundsData[i].m_radius)));
            }
        FE_END_PROFILE;
    }

void fe::particleSystem::setParticleWeight(float weight)
    {
        m_particleWeight = weight;
    }

void fe::particleSystem::setAverageWeightUntilRepulsion(float weight)
    {
        m_weightAverage = weight;
    }

void fe::particleSystem::setRepulsionCoefficient(float coef)
    {
        m_repulsionCoefficient = coef;
    }

void fe::particleSystem::queueParticles(fe::time lifetime, fe::particleFlags flags, sf::Color colour, float particleRadius, unsigned int count, fe::Vector2d position, float speed, float arc, float heading)
    {
        queueParticles(lifetime, flags, colour, particleRadius, count, position, 0.f, speed, arc, heading);
    }

void fe::particleSystem::queueParticles(fe::time lifetime, fe::particleFlags flags, sf::Color colour, float particleRadius, unsigned int count, fe::Vector2d position, float radius, float speed, float arc, float heading)
    {
        constexpr float degToRad = 3.14159f / 180.f;

        float fov = arc / 2;
        arc = arc * degToRad;
        heading = heading * degToRad;
        fov = fov * degToRad;
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
