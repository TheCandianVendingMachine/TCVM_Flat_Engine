// eventSender.hpp
// This allows any part in a program to communicate with another part via messages
#pragma once
#include "../../flatEngineExport.hpp"

#include <vector>
#include <queue>
#include <functional>
#include <unordered_map>

#include "gameEvent.hpp"
#include "../../time/clock.hpp"
#include "../../typeDefines.hpp"

namespace fe
    {
        class eventHandler;

        class eventSender
            {
                private:
                    std::unordered_map<unsigned int, eventHandler*> m_handlers;
                    std::unordered_map<fe::int64, std::vector<eventHandler*>> m_observers;

                    std::priority_queue<gameEvent, std::vector<gameEvent>, std::less_equal<gameEvent&>>   m_eventQueueFrame;
                    std::priority_queue<gameEvent, std::vector<gameEvent>, std::less<gameEvent&>>         m_eventQueueTimed;

                    unsigned int m_currentFrame;
                    fe::clock m_elapsedTime;

                    FLAT_ENGINE_API void sendEvent(gameEvent &msg);

                public:
                    FLAT_ENGINE_API eventSender();

                    FLAT_ENGINE_API void subscribe(eventHandler *handler);
                    FLAT_ENGINE_API void subscribe(eventHandler *handler, fe::int64 messageType);

                    FLAT_ENGINE_API void unsubscribe(eventHandler *handler);
                    FLAT_ENGINE_API void unsubscribe(eventHandler *handler, fe::int64  messageType);

                    FLAT_ENGINE_API void unsubscribeAll(eventHandler *handler);

                    FLAT_ENGINE_API void clear();

                    /* This block of functions sends the event to a specific person. Allows arguments for time */
                    FLAT_ENGINE_API void sendTo(gameEvent event, unsigned int to);
                    FLAT_ENGINE_API void sendTo(gameEvent event, unsigned int to, float time);
                    FLAT_ENGINE_API void sendTo(gameEvent event, unsigned int to, int frame);

                    /* This block of functions sends the event to a specific event subscriber. Allows arguments for time */
                    FLAT_ENGINE_API void send(gameEvent event, fe::int64 eventType);
                    FLAT_ENGINE_API void send(gameEvent event, fe::int64 eventType, float time);
                    FLAT_ENGINE_API void send(gameEvent event, fe::int64 eventType, int frame);

                    /* This block of functions sends the event to a everyone. Allows arguments for time */
                    FLAT_ENGINE_API void sendGlobal(gameEvent event);
                    FLAT_ENGINE_API void sendGlobal(gameEvent event, float time);
                    FLAT_ENGINE_API void sendGlobal(gameEvent event, int frame);

                    /* Thiese functions sends a game engine event. This event does not have an offset applied to it */
                    FLAT_ENGINE_API void sendEngineEvent(gameEvent event, fe::int64 eventType);
                    FLAT_ENGINE_API void sendEngineEvent(gameEvent event, fe::int64 eventType, float time);
                    FLAT_ENGINE_API void sendEngineEvent(gameEvent event, fe::int64 eventType, int frame);

                    FLAT_ENGINE_API void sendEvents();

                    FLAT_ENGINE_API ~eventSender() = default;

            };

    }