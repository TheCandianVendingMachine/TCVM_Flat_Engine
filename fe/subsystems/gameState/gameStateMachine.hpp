// gameStateMachine.hpp
// the FSM for the game states
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/stackAllocater.hpp"

namespace sf
    {
        class RenderTarget;
        class Event;
    }

namespace fe
    {
        class baseGameState;
        class sceneGraph;
        
        class gameStateMachine
            {
                public:
                    enum stateOptions
                        {
                            NONE                = 0,
                            RENDER_OVERTOP      = 1 << 0,
                            UPDATE_UNDERNEATH   = 1 << 1
                        };

                private:
                    struct stateList
                        {
                            stateList *m_head;
                            stateList *m_tail;

                            stateOptions m_options;

                            baseGameState *m_currentState;
                            unsigned int m_offset;
                        };

                    // since all memory that the state has will be irrelevant once we pop it, we get the current stack marker to free to
                    fe::stackAllocater::Marker m_stateMarker; // this marker is from m_previousState
                    fe::stackAllocater m_stateAllocater;
                    bool m_pop;
                    bool m_clear;

                    stateList *m_endState; // last state in the linked list

                    stateOptions m_nestStateOptions;
                    baseGameState *m_nextState;


                    // Since the game needs to fully initialize, we don't update for one tick. Once the tick is done, this flag is true and
                    // we update as usual
                    bool m_update;

                    static gameStateMachine *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API gameStateMachine &get();

                    // push the state as the current state
                    FLAT_ENGINE_API void push(baseGameState *newState, stateOptions options = stateOptions::NONE);
                    // pop the current state
                    FLAT_ENGINE_API void pop();

                    // clear all states off the list
                    FLAT_ENGINE_API void clear();

                    // Queue a push to happen next frame
                    template<typename T>
                    void queuePush(stateOptions options = stateOptions::NONE);
                    // Queue a pop to happen next frame
                    FLAT_ENGINE_API void queuePop();
                    // Queue a clear to happen next frame
                    FLAT_ENGINE_API void queueClear();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);
                    FLAT_ENGINE_API void postDraw();

                    FLAT_ENGINE_API fe::sceneGraph &getSceneGraph();

                    FLAT_ENGINE_API const fe::baseGameState &getCurrentState();

                    FLAT_ENGINE_API virtual ~gameStateMachine() {}

            };

        template<typename T>
        void gameStateMachine::queuePush(stateOptions options)
            {
                m_nextState = new T();
                m_nestStateOptions = options;
            }
    }