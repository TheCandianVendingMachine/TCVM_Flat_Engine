#include "systemModule.hpp"

fe::systemModule::systemModule()
    {
        m_shutDown = false;
    }

fe::systemModule::~systemModule()
    {
        FE_ASSERT(m_shutDown, "Error: Module not shut down safely: Possible memory leaks");
    }
