// random.hpp
// allows for easy randomization of items
#pragma once
#include "../flatEngineExport.hpp"
#include <random>
#include <limits>

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

            };

        template<typename T>
        inline T random::generate(T min, T max)
            {
                if (m_hasSeed)
                    {
                        m_randomizer.seed(m_seed);
                    }

                if constexpr (std::is_floating_point<T>::value) 
                    {
                        std::uniform_real_distribution<T> dist(min, max);
                        return dist(m_randomizer);
                    }
                else if constexpr(std::numeric_limits<T>::is_integer)
                    {
                        std::uniform_int_distribution<T> dist(min, max);
                        return dist(m_randomizer);
                    }

                return T();
            }
    }