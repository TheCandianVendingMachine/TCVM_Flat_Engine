// particleSystem.hpp
// manages, simulates, and handles all particles
// Inspired by Google's LiquidFun (http://google.github.io/liquidfun/)
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/dataStructures/doublyLinkedList.hpp"
#include "fe/subsystems/collision/bounds/circle.hpp"
#include "fe/time/time.hpp"
#include "particleFlags.hpp"
#include "particleGroup.hpp"
#include "particleBatch.hpp"
#include <vector>
#include <unordered_map>
#include <queue>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class particleSystem
            {
                private:
                    using particleNode = fe::doublyLinkedList<fe::particleGroup>::node;
                    using particle = unsigned int;
                    
                    // basic particle data used for spawning new particles
                    struct particleData
                        {
                            fe::time m_deathTime;
                            fe::Vector2d m_velocity;
                            fe::Vector2d m_position;
                            sf::Color m_colour;
                            float m_radius;
                            fe::particleFlags m_flags;
                        };

                    // Collision data for particles to compute collision pairs
                    // Vector looks like:
                    // [(0, 4, true), (1, 0, false), (2, 0, false), (3, 0, false), (4, 0, true)]
                    struct particleCollisionData
                        {
                            particle m_particle; // actual particle index
                            bool m_collider = false; // whether or not this is the one being tested

                            particleCollisionData(particle p) : m_particle(p) {}
                        };

                    std::queue<particleData> m_queuedParticles;


                    // Inline arrays that define a particle.
                    // Particles are defined as just having a velocity, boundary, and a flag
                    std::vector<fe::Vector2d> m_particleVelocities;
                    std::vector<fe::circle> m_particleBounds;
                    std::vector<fe::particleFlags> m_particleFlags;
                    std::vector<fe::time> m_deathTime;
                    std::vector<sf::Color> m_colour;
                    std::vector<particle> m_collisionParticles;
                    std::vector<particle> m_particles;

                    unsigned int m_totalParticles;

                    std::vector<particleCollisionData> m_collisionPairs;

                    std::vector<fe::particleGroup> m_collisionGroups;
                    fe::doublyLinkedList<fe::particleGroup> m_groupList;

                    fe::particleBatch m_batch;

                    FLAT_ENGINE_API void sortParticles(const fe::circle *boundsData, const unsigned int *collisionParticlesData);
                    FLAT_ENGINE_API void broadphase(const unsigned int *collisionParticlesData, const fe::circle *boundsData);

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void determineCollisionPairs();
                    FLAT_ENGINE_API void preUpdate(fe::time currentTime);
                    FLAT_ENGINE_API void fixedUpdate(float dt);

                    FLAT_ENGINE_API void queueParticles(fe::time lifetime, fe::particleFlags flags, sf::Color colour, float particleRadius, unsigned int count, fe::Vector2d position, float speed, float arc, float heading);
                    FLAT_ENGINE_API void queueParticles(fe::time lifetime, fe::particleFlags flags, sf::Color colour, float particleRadius, unsigned int count, fe::Vector2d position, float radius, float speed, float arc, float heading);

                    FLAT_ENGINE_API void draw(sf::RenderTarget &target);

                    FLAT_ENGINE_API unsigned int getTotalParticles() const;

            };
    }

