// particleSystem.hpp
// manages, simulates, and handles all particles
// Inspired by Google's LiquidFun (http://google.github.io/liquidfun/)
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/dataStructures/doublyLinkedList.hpp"
#include "fe/subsystems/collision/bounds/circle.hpp"
#include "particleFlags.hpp"
#include "particleGroup.hpp"
#include <vector>
#include <unordered_map>

namespace fe
    {
        class particleSystem
            {
                private:
                    using particleNode = fe::doublyLinkedList<fe::particleGroup>::node;
                    using particle = unsigned int;
                    

                    // Inline arrays that define a particle.
                    // Particles are defined as just having a position, velocity, boundary, and a flag
                    std::vector<fe::Vector2d> m_particlePositions;
                    std::vector<fe::Vector2d> m_particleVelocities;
                    std::vector<fe::circle> m_particleBounds;
                    std::vector<fe::particleFlags> m_particleFlags;
                    std::vector<int> m_particleCollisionGroup; // current collision group of the particle data = index in m_collisionGroups
                    std::vector<particle> m_particles;

                    unsigned int m_totalParticles;
                    unsigned int m_activeParticles;

                    std::unordered_map<particle, std::vector<particle>> m_collisionPairs;
                    std::unordered_map<particle, std::vector<particle>> m_potentialCollisions;

                    fe::doublyLinkedList<fe::particleGroup> m_collisionGroups;
                    fe::doublyLinkedList<fe::particleGroup> m_groupList;

                    void determinePossibleCollisions(particle particle, particleNode *node);
                    void sortParticles();
                    void broadphase();

                public:
                    void startUp();
                    void shutDown();
                    void determineCollisionPairs();
                    void fixedUpdate(float dt);

                    unsigned int getTotalParticles() const;

            };
    }

