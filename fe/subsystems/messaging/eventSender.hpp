// eventSender.hpp
// This allows any part in a program to communicate with another part via messages
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

#include <vector>
#include <queue>
#include <functional>
#include <unordered_map>

#include "gameEvent.hpp"
#include "../../time/clock.hpp"

namespace fe
    {
        class eventHandler;

        class eventSender
            {
                private:
                    std::unordered_map<unsigned int, eventHandler*> m_handlers;
                    std::unordered_map<int, std::vector<eventHandler*>> m_observers;

                    std::priority_queue<gameEvent, std::vector<gameEvent>, std::less_equal<gameEvent&>>   m_eventQueueFrame;
                    std::priority_queue<gameEvent, std::vector<gameEvent>, std::less<gameEvent&>>         m_eventQueueTimed;

                    unsigned int m_currentFrame;
                    fe::clock m_elapsedTime;

                    FLAT_ENGINE_API void sendEvent(gameEvent &msg);

                public:
                    FLAT_ENGINE_API void subscribe(eventHandler *handler);
                    FLAT_ENGINE_API void subscribe(eventHandler *handler, int messageType);

                    FLAT_ENGINE_API void unsubscribe(eventHandler *handler);
                    FLAT_ENGINE_API void unsubscribe(eventHandler *handler, int messageType);

                    FLAT_ENGINE_API void unsubscribeAll(eventHandler *handler);

                    FLAT_ENGINE_API void clear();

                    /* This block of functions sends the event to a specific person. Allows arguments for time */
                    FLAT_ENGINE_API void send(gameEvent event, unsigned int to);
                    FLAT_ENGINE_API void send(gameEvent event, unsigned int to, float time);
                    FLAT_ENGINE_API void send(gameEvent event, unsigned int to, int frame);

                    /* This block of functions sends the event to a specific event subscriber. Allows arguments for time */
                    FLAT_ENGINE_API void send(gameEvent event, int eventType);
                    FLAT_ENGINE_API void send(gameEvent event, int eventType, float time);
                    FLAT_ENGINE_API void send(gameEvent event, int eventType, int frame);

                    /* This block of functions sends the event to a everyone. Allows arguments for time */
                    FLAT_ENGINE_API void sendGlobal(gameEvent event);
                    FLAT_ENGINE_API void sendGlobal(gameEvent event, float time);
                    FLAT_ENGINE_API void sendGlobal(gameEvent event, int frame);

                    FLAT_ENGINE_API void sendEvents();

            };

    }