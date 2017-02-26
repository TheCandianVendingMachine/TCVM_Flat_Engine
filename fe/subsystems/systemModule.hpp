// systemModule.hpp
// a virtual base class to allow sub-systems to have startup/shutdown routines
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineInclude.hpp"

namespace fe
    {
        class FLAT_ENGINE_API systemModule
            {
                protected:
                    bool m_shutDown;

                public:
                    systemModule();

                    virtual void startUp() = 0;
                    virtual void shutDown() = 0;

                    virtual ~systemModule();
            };
    }