#include "fe/math/random.hpp"
#include "fe/feAssert.hpp"
#include "fe/typeDefines.hpp"

fe::random *fe::random::m_instance = nullptr;

void fe::random::startUp()
    {
        FE_ASSERT((m_instance == nullptr), "Randomizer instance already created!");
        if (!m_instance)
            {
                m_instance = this;
        #ifdef _DEBUG and FE_DEBUG_NO_SEED
                m_seed = FE_DEFAULT_RANDOM_SEED;
        #else
                m_seed = std::random_device{}();
        #endif
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
