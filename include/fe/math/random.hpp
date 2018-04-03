// random.hpp
// allows for easy randomization of items
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include <random>

namespace fe
    {
        class random
            {
                private:
                    static random *m_instance;
                    std::mt19937 m_randomizer;

                    unsigned int m_seed;
                    bool m_hasSeed;

                public:
                    FLAT_ENGINE_API void startUp();

                    FLAT_ENGINE_API void useSeed(bool value);
                    FLAT_ENGINE_API void setSeed(unsigned int seed = 0);

                    FLAT_ENGINE_API static random &get();
                    template<typename T>
                    T generate(T min, T max);
                    template<>
                    int generate(int min, int max);
                    template<>
                    unsigned int generate(unsigned int min, unsigned int max);

            };

        template<typename T>
        inline T random::generate(T min, T max)
            {
                if (m_hasSeed)
                    {
                        m_randomizer.seed(m_seed);
                    }

                std::uniform_real_distribution<T> dist(min, max);
                return dist(m_randomizer);
            }

        template<>
        inline int random::generate(int min, int max)
            {
                if (m_hasSeed)
                    {
                        m_randomizer.seed(m_seed);
                    }

                std::uniform_int_distribution<int> dist(min, max);
                return dist(m_randomizer);
            }

        template<>
        inline unsigned int random::generate(unsigned int min, unsigned int max)
            {
                if (m_hasSeed)
                    {
                        m_randomizer.seed(m_seed);
                    }

                std::uniform_int_distribution<unsigned int> dist(min, max);
                return dist(m_randomizer);
            }
    }