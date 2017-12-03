// gameStateMachine.hpp
// the FSM for the game states
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../memory/stackAllocater.hpp"
#include <tuple>
#include <utility>

namespace sf
    {
        class RenderTarget;
        class Event;
    }

namespace fe
    {
        class baseGameState;
        class sceneGraph;
        class collisionWorld;
        class broadphaseAbstract;
        
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

                    struct stateHolderBase { virtual void *construct() { return nullptr; } };
                    template<typename T, typename ...Args>
                    struct stateHolder : stateHolderBase 
                        {
                            std::tuple<Args...> args;

                            stateHolder(Args &&...arg) : args(std::forward<Args>(arg)...) { }
                            void *construct() { return constructT(args, std::index_sequence_for<Args...>()); }

                            private:
                                template<std::size_t... S>
                                void *constructT(std::tuple<Args...> &tuple, std::index_sequence<S...>) { return new T(std::get<S>(tuple)...); }
                        };

                    fe::stackAllocater m_stateAllocater;
                    bool m_pop;
                    bool m_clear;

                    stateList *m_endState; // last state in the linked list

                    stateOptions m_nextStateOptions;
                    stateHolderBase *m_nextState;


                    // Since the game needs to fully initialize, we don't update for one tick. Once the tick is done, this flag is true and
                    // we update as usual
                    bool m_update;

                    static gameStateMachine *m_instance;

                    FLAT_ENGINE_API void drawState(stateList *node, sf::RenderTarget &app);

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
                    template<typename T, typename ...Args>
                    void queuePush(stateOptions options = stateOptions::NONE, Args &&...args);
                    // Queue a pop to happen next frame
                    FLAT_ENGINE_API void queuePop();
                    // Queue a clear to happen next frame
                    FLAT_ENGINE_API void queueClear();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(collisionWorld *collisionWorld);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);
                    FLAT_ENGINE_API void postDraw();

                    FLAT_ENGINE_API const fe::sceneGraph &getSceneGraph() const;
                    FLAT_ENGINE_API const fe::baseGameState &getCurrentState() const;
                    FLAT_ENGINE_API const fe::broadphaseAbstract *getDynamicBroadphase() const;
                    FLAT_ENGINE_API const fe::broadphaseAbstract *getStaticBroadphase() const;

                    FLAT_ENGINE_API fe::sceneGraph &getSceneGraph();
                    FLAT_ENGINE_API fe::baseGameState &getCurrentState();
                    FLAT_ENGINE_API fe::broadphaseAbstract *getDynamicBroadphase();
                    FLAT_ENGINE_API fe::broadphaseAbstract *getStaticBroadphase();


                    FLAT_ENGINE_API virtual ~gameStateMachine() {}

            };

        template<typename T, typename ...Args>
        void gameStateMachine::queuePush(stateOptions options, Args &&...args)
            {
                m_nextState = new(m_stateAllocater.alloc(sizeof(stateHolder<T, Args...>))) stateHolder<T, Args...>(std::forward<Args>(args)...);
                m_nextStateOptions = options;
            }
    }