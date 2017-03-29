// eventHandler.hpp
// allows this class to handle events
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

namespace fe
    {
        struct gameEvent;
        class eventHandler
            {
                private:
                    static unsigned int m_lastId;
                    unsigned int m_id;

                public:
                    FLAT_ENGINE_API eventHandler();
                    FLAT_ENGINE_API eventHandler(unsigned int id);

                    virtual void handleEvent(const gameEvent &event) = 0;
                    unsigned int id() { return m_id; }

                    FLAT_ENGINE_API bool operator==(const eventHandler &rhs);

            };
    }