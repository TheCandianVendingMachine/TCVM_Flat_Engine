// gameEvent.hpp
// a struct containing message data
#pragma once
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"

namespace fe
    {
        enum class sendType
            {
                SEND_TO_TARGET,
                SEND_TO_EVENT,
                SEND_TO_ALL
            };

        struct gameEventArgument
            {
                enum class type
                    {
                        TYPE_INT,
                        TYPE_UINT,
                        TYPE_FLOAT,
                        TYPE_VOIDP,  
                        TYPE_BOOL,
                        TYPE_STRING,
                    };

                union argument
                    {
                        int             TYPE_INTEGER;
                        unsigned int    TYPE_UINTEGER;
                        float           TYPE_FLOAT;
                        void*           TYPE_VOIDP;
                        bool            TYPE_BOOL;
                    };
                argument arg;
                type argType;

                gameEventArgument() {}
                gameEventArgument(float var) : argType(type::TYPE_FLOAT) { arg.TYPE_FLOAT = var; }
                gameEventArgument(int var) : argType(type::TYPE_INT) { arg.TYPE_INTEGER = var; }
                gameEventArgument(unsigned int var) : argType(type::TYPE_UINT) { arg.TYPE_UINTEGER = var; }
                gameEventArgument(void* var) : argType(type::TYPE_VOIDP) { arg.TYPE_VOIDP = var; }
            };

        struct gameEvent
            {
                static constexpr unsigned int ARGUMENT_COUNT = 8;

                sendType sendType;      // the type which this event is initialized under

                fe::int64 eventType;          // the event which will be used
                unsigned int argNumber; // how many arguments in array
                unsigned int target;    // target ID of the reciever
                unsigned int eventSent; // time when the event was sent in milliseconds
                gameEventArgument args[ARGUMENT_COUNT];

                gameEvent() = default;
                gameEvent(fe::int64 event, unsigned int args) : eventType(event), argNumber(args) {}

                FLAT_ENGINE_API bool operator<=(const gameEvent &rhs) const; // used in eventMessanger. Used to check if this frametime is lower than the next message's
                FLAT_ENGINE_API bool operator<(const gameEvent &rhs) const;  // used in eventMessanger. Used to check if this time is lower than the next message's
                FLAT_ENGINE_API gameEvent operator=(const gameEvent &rhs);

                private:
                    enum class timeVar
                        {
                            NONE,
                            FRAME,
                            TIME
                        };
        
                    union timeType
                        {
                            int     FRAME;
                            float   TIME;
                        };

                    timeVar sendTimeType;   // Variable of time in union
                    timeType sendTimeVar;   // Union Variable

                    friend class eventSender;
            };
    }