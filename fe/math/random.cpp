#include "random.hpp"
#include "../feAssert.hpp"

fe::random *fe::random::m_instance = nullptr;

void fe::random::startUp()
    {
        FE_ASSERT((m_instance == nullptr), "Randomizer instance already created!");
        if (!m_instance)
            {
                m_instance = this;
                m_randomizer.seed(std::random_device{}());
            }
    }

void fe::random::useSeed(bool value)
    {
        m_hasSeed = value;
    }

void fe::random::setSeed(unsigned int seed)
    {
        m_seed = seed;
    }

fe::random &fe::random::get()
    {
        return *m_instance;
    }
