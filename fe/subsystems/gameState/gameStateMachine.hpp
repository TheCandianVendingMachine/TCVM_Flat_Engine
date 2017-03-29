// gameStateMachine.hpp
// the FSM for the game states
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/stackAllocater.hpp"

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class baseGameState;
        class sceneGraph;
        
        class gameStateMachine
            {
                private:
                    // since all memory that the state has will be irrelevant once we pop it, we get the current stack marker to free to
                    fe::stackAllocater::Marker m_stateMarker; // this marker is from m_previousState
                    bool m_pop;

                    baseGameState *m_currentState;
                    baseGameState *m_nextState;

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
                    FLAT_ENGINE_API void queuePush(baseGameState *newState);
                    // Queue a pop to happen next frame
                    FLAT_ENGINE_API void queuePop();

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderWindow &app);
                    FLAT_ENGINE_API void postDraw();

                    FLAT_ENGINE_API fe::sceneGraph &getSceneGraph();

                    FLAT_ENGINE_API const fe::baseGameState &getCurrentState();

                    FLAT_ENGINE_API virtual ~gameStateMachine() {}

            };
    }