#include "fe/subsystems/gameState/gameStateMachine.hpp"
#include "fe/subsystems/gameState/gameState.hpp"
#include "fe/subsystems/memory/memoryManager.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/engine.hpp"
#include "fe/debug/profiler.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::gameStateMachine *fe::gameStateMachine::m_instance = nullptr;

void fe::gameStateMachine::startUp()
    {
        if (!m_instance) 
            {
                const unsigned int maxStates = 32;

                auto size = maxStates * sizeof(stateList);
                auto memBuf = FE_ALLOC_DIRECT_CAPTURED("StateBuffer", size);
                m_stateAllocater.startUp(static_cast<fe::uInt8*>(memBuf), size);

                m_instance = this;

                m_pop = false;
                m_nextState = nullptr;

                m_update = true;

                m_endState = nullptr;
            }
    }

void fe::gameStateMachine::shutDown()
    {
        clear();
    }

fe::gameStateMachine &fe::gameStateMachine::get()
    {
        return *m_instance;
    }

void fe::gameStateMachine::push(baseGameState *newState, stateOptions options)
    {
        m_nextState = nullptr;
        if (m_endState) 
            {
                if (m_endState->m_currentState)
                    {
                        // If the end state option wants to be updated underneath, this test will return something > 0. Therefore we check if it is zero to pause
                        m_endState->m_currentState->pause((m_endState->m_options & stateOptions::UPDATE_UNDERNEATH) == 0);
                        m_endState->m_currentState->onDeactive();
                    }
                
                m_endState->m_headMarker = m_stateAllocater.getMarker();
                auto listBuf = m_stateAllocater.alloc(sizeof(stateList));
                m_endState->m_head = new(listBuf) stateList();

                auto tail = m_endState;

                m_endState = m_endState->m_head;
                m_endState->m_tail = tail;
            }
        else
            {
                auto listBuf = m_stateAllocater.alloc(sizeof(stateList));
                m_endState = new(listBuf) stateList;

                m_endState->m_head = nullptr;
                m_endState->m_tail = nullptr;
            }

        m_endState->m_currentState = newState;
        m_endState->m_currentState->startUp();
        
        m_endState->m_currentState->init();
        m_endState->m_currentState->onActive();
        m_endState->m_options = options;

        // we set the offset here so we can free the memory past the previous state, and behind the current
        m_endState->m_offset = fe::memoryManager::get().getStackAllocater().getMarker();

        m_endState->m_paused = m_endState->m_currentState->isPaused();

        m_update = false;
    }

void fe::gameStateMachine::pop()
    {
        if (m_endState)
            {
                m_endState->m_currentState->onDeactive();
                m_endState->m_currentState->deinit();
                m_endState->m_currentState->shutDown();
                delete m_endState->m_currentState;

                auto offset = m_endState->m_offset;
                if (m_endState->m_tail)
                    {
                        m_endState = m_endState->m_tail;
                    }
                else
                    {
                        m_stateAllocater.freeToMarker(m_endState->m_headMarker);
                        m_endState = nullptr;
                    }

                if (m_endState)
                    {
                        if (m_endState->m_head) 
                            {
                                m_stateAllocater.freeToMarker(m_endState->m_headMarker);
                                //delete m_endState->m_head;
                            }
                        m_endState->m_head = nullptr;

                        FE_FREE_STACK("GameStateMachine", offset);

                        if (m_endState->m_currentState)
                            {
                                m_endState->m_currentState->onActive();
                            }

                        m_endState->m_currentState->pause(m_endState->m_paused);
                    }
            }
    }

void fe::gameStateMachine::clear()
    {
        while (m_endState)
            {
                pop();
            }
    }

void fe::gameStateMachine::queuePop()
    {
        m_pop = true;
    }

void fe::gameStateMachine::queueClear()
    {
        m_clear = true;
    }

void fe::gameStateMachine::handleEvents(const sf::Event &event)
    {
        if (m_endState && m_endState->m_currentState)
            {
                m_endState->m_currentState->handleEvents(event);
            }
    }

void fe::gameStateMachine::preUpdate()
    {
        if (m_clear)
            {
                clear();
                m_pop = false;
                m_clear = false;
            }

        if (m_pop)
            {
                pop();
                m_pop = false;
            }

        if (m_nextState)
            {
                push(static_cast<fe::baseGameState*>(m_nextState->construct()), m_nextStateOptions);
            }

        FE_ENGINE_PROFILE("gameStateMachine", "preupdateUserState");
        if (m_endState && m_endState->m_currentState)
            {
                m_endState->m_currentState->preUpdateDefined();

                stateList *tail = m_endState->m_tail;
                while (tail && tail->m_currentState && tail->m_options & stateOptions::UPDATE_UNDERNEATH)
                    {
                        tail->m_currentState->preUpdate();
                        tail = tail->m_tail;
                    }
            }
        FE_END_PROFILE;
    }

void fe::gameStateMachine::update(collisionWorld *collisionWorld)
    {
        FE_ENGINE_PROFILE("gameStateMachine", "updateUserState");
        if (m_endState && m_endState->m_currentState && m_update)
            {
                m_endState->m_currentState->updateDefined(collisionWorld);

                stateList *tail = m_endState->m_tail;
                while (tail && tail->m_currentState && tail->m_options & stateOptions::UPDATE_UNDERNEATH)
                    {
                        tail->m_currentState->updateDefined(collisionWorld);
                        tail = tail->m_tail;
                    }
            }
        FE_END_PROFILE;
    }

void fe::gameStateMachine::postUpdate()
    {
        FE_ENGINE_PROFILE("gameStateMachine", "postupdateUserState");
        if (m_endState && m_endState->m_currentState)
            {
                m_endState->m_currentState->postUpdate();
                m_endState->m_currentState->postUpdateDefined();

                stateList *tail = m_endState->m_tail;
                while (tail && tail->m_currentState && tail->m_options & stateOptions::UPDATE_UNDERNEATH)
                    {
                        tail->m_currentState->postUpdate();
                        tail->m_currentState->postUpdateDefined();
                        tail = tail->m_tail;
                    }
            }

        if (!m_update)
            {
                m_update = true;
            }
        FE_END_PROFILE;
    }

void fe::gameStateMachine::cameraUpdate(float deltaTime, int iterations)
    {
        // Only updating end state camera as for that makes the most sense. No other camera should be moving
        m_endState->m_currentState->updateCamera(deltaTime, iterations);
    }

void fe::gameStateMachine::preDraw()
    {
        FE_ENGINE_PROFILE("gameStateMachine", "predrawUserState");
        if (m_endState && m_endState->m_currentState)
            {
                m_endState->m_currentState->preDraw();
                m_endState->m_currentState->preDrawDefined();

                stateList *listTop = m_endState;
                stateList *tail = m_endState->m_tail;
                while (listTop->m_options & stateOptions::RENDER_OVERTOP && tail && tail->m_currentState)
                    {
                        tail->m_currentState->preDraw();

                        listTop = tail;
                        tail = tail->m_tail;
                    }
            }
        FE_END_PROFILE;
    }

void fe::gameStateMachine::drawState(stateList *node, sf::RenderTarget &app)
    {
        if (node && node->m_options & RENDER_OVERTOP)
            {
                drawState(node->m_tail, app);
            }
        node->m_currentState->draw(app);
    }

void fe::gameStateMachine::draw(sf::RenderTarget &app)
    {
        FE_ENGINE_PROFILE("gameStateMachine", "drawUserState");
        if (m_endState && m_endState->m_currentState)
            {
                app.setView(m_endState->m_currentState->getCamera().getView());
                drawState(m_endState, app);
            }
        FE_END_PROFILE;
    }

void fe::gameStateMachine::postDraw()
    {
        FE_ENGINE_PROFILE("gameStateMachine", "postdrawUserState");
        if (m_endState && m_endState->m_currentState)
            {
                m_endState->m_currentState->postDraw();

                stateList *listTop = m_endState;
                stateList *tail = m_endState->m_tail;
                while (listTop->m_options & stateOptions::RENDER_OVERTOP && tail && tail->m_currentState)
                    {
                        tail->m_currentState->postDraw();

                        listTop = tail;
                        tail = tail->m_tail;
                    }
            }
        FE_END_PROFILE;
    }

const fe::sceneGraph &fe::gameStateMachine::getSceneGraph() const
    {
        return m_endState->m_currentState->getGameWorld().getSceneGraph();
    }

const fe::baseGameState &fe::gameStateMachine::getCurrentState() const
    {
        return *m_endState->m_currentState;
    }

const fe::broadphaseAbstract *fe::gameStateMachine::getDynamicBroadphase() const
    {
        if (!m_endState) return nullptr;
        return m_endState->m_currentState->getGameWorld().getDynamicBroadphase();
    }

const fe::broadphaseAbstract *fe::gameStateMachine::getStaticBroadphase() const
    {
        if (!m_endState) return nullptr;
        return m_endState->m_currentState->getGameWorld().getStaticBroadphase();
    }

fe::sceneGraph &fe::gameStateMachine::getSceneGraph()
    {
        return m_endState->m_currentState->getGameWorld().getSceneGraph();
    }

fe::baseGameState &fe::gameStateMachine::getCurrentState()
    {
        return *m_endState->m_currentState;
    }

fe::broadphaseAbstract *fe::gameStateMachine::getDynamicBroadphase()
    {
        if (!m_endState) return nullptr;
        return m_endState->m_currentState->getGameWorld().getDynamicBroadphase();
    }

fe::broadphaseAbstract *fe::gameStateMachine::getStaticBroadphase()
    {
        if (!m_endState) return nullptr;
        return m_endState->m_currentState->getGameWorld().getStaticBroadphase();
    }

