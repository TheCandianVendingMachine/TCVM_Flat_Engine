// eventHandler.hpp
// allows this class to handle events
#pragma once
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
                    unsigned int eventHandlerId() { return m_id; }

                    FLAT_ENGINE_API bool operator==(const eventHandler &rhs);

            };
    }