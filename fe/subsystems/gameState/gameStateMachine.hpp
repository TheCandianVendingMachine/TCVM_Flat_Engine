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
                private:
                    struct stateList
                        {
                            stateList *m_head;
                            stateList *m_tail;

                            baseGameState *m_currentState;
                            unsigned int m_offset;
                        };

                    // since all memory that the state has will be irrelevant once we pop it, we get the current stack marker to free to
                    fe::stackAllocater::Marker m_stateMarker; // this marker is from m_previousState
                    fe::stackAllocater m_stateAllocater;
                    bool m_pop;

                    stateList *m_baseState;
                    stateList *m_endState; // last state in the linked list

                    // Since the game needs to fully initialize, we don't update for one tick. Once the tick is done, this flag is true and
                    // we update as usual
                    bool m_update;

                    static gameStateMachine *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API gameStateMachine &get();

                    // push the state as the current state
                    FLAT_ENGINE_API void push(baseGameState *newState);
                    // pop the current state
                    FLAT_ENGINE_API void pop();

                    // Queue a push to happen next frame
                    template<typename T>
                    void queuePush();
                    // Queue a pop to happen next frame
                    FLAT_ENGINE_API void queuePop();

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
        void gameStateMachine::queuePush()
            {
                m_nextState = new T();
            }
    }