#include "gameStateMachine.hpp"
#include "../memory/memoryManager.hpp"
#include "../messaging/eventSender.hpp"
#include "../../engine.hpp"
#include "gameState.hpp"

fe::gameStateMachine *fe::gameStateMachine::m_instance = nullptr;

void fe::gameStateMachine::drawState(stateList *node, sf::RenderTarget &app)
    {
        if (node && node->m_options & RENDER_OVERTOP)
            {
                drawState(node->m_tail, app);
            }
        node->m_currentState->draw(app);
    }

void fe::gameStateMachine::startUp()
    {
        if (!m_instance) 
            {
                const unsigned int maxStates = 32;
                auto size = sizeof(stateHolderBase) * maxStates;

                auto memBuf = FE_ALLOC_DIRECT_CAPTURED("StateBuffer", size);
                m_stateAllocater.startUp(static_cast<char*>(memBuf), size);

                m_stateMarker = fe::memoryManager::get().getStackAllocater().getMarker();

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
        if (m_endState) 
            {
                m_endState->m_offset = fe::memoryManager::get().getStackAllocater().getMarker();
                auto listBuf = FE_ALLOC_STACK("StateListAlloc", sizeof(stateList));
                m_endState->m_head = new(listBuf) stateList();

                auto tail = m_endState;

                m_endState = m_endState->m_head;
                m_endState->m_tail = tail;
            }
        else
            {
                auto listBuf = FE_ALLOC_STACK("StateListAlloc", sizeof(stateList));
                m_endState = new(listBuf) stateList;

                m_endState->m_head = nullptr;
                m_endState->m_tail = nullptr;
            }

        m_endState->m_currentState = newState;
        m_endState->m_currentState->init();
        m_endState->m_options = options;

        m_update = false;

        // we set the marker to the top of the stack after we push because we want to capture
        // all of the STL's allocations in construction as well so when things are destructed
        // they are still in memory. We have a small "leak" but that is worth it
        m_stateMarker = fe::memoryManager::get().getStackAllocater().getMarker();
    }

void fe::gameStateMachine::pop()
    {
        if (m_endState)
            {
                m_endState->m_currentState->deinit();
                delete m_endState->m_currentState;

                if (m_endState->m_tail)
                    {
                        m_endState = m_endState->m_tail;
                    }
                else
                    {
                        delete m_endState;
                        m_endState = nullptr;
                    }

                if (m_endState)
                    {
                        if (m_endState->m_head) 
                            {
                                delete m_endState->m_head;
                            }
                        m_endState->m_head = nullptr;

                        FE_FREE_STACK("GameStateMachine", m_endState->m_offset);
                    }
                else
                    {
                        FE_FREE_STACK("GameStateMachine", m_stateMarker);
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
                m_nextState = nullptr;
            }

        if (m_endState && m_endState->m_currentState)
            {
                m_endState->m_currentState->preUpdate();

                stateList *tail = m_endState->m_tail;
                while (tail && tail->m_currentState && tail->m_options & stateOptions::UPDATE_UNDERNEATH)
                    {
                        tail->m_currentState->preUpdate();
                        tail = tail->m_tail;
                    }
            }
    }

void fe::gameStateMachine::update(float deltaTime)
    {
        if (m_endState && m_endState->m_currentState && m_update)
            {
                m_endState->m_currentState->update(deltaTime);

                stateList *tail = m_endState->m_tail;
                while (tail && tail->m_currentState && tail->m_options & stateOptions::UPDATE_UNDERNEATH)
                    {
                        tail->m_currentState->update(deltaTime);
                        tail = tail->m_tail;
                    }
            }
    }

void fe::gameStateMachine::postUpdate()
    {
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
    }

void fe::gameStateMachine::preDraw()
    {
        if (m_endState && m_endState->m_currentState)
            {
                m_endState->m_currentState->preDraw();

                stateList *listTop = m_endState;
                stateList *tail = m_endState->m_tail;
                while (listTop->m_options & stateOptions::RENDER_OVERTOP && tail && tail->m_currentState)
                    {
                        tail->m_currentState->preDraw();

                        listTop = tail;
                        tail = tail->m_tail;
                    }
            }
    }

void fe::gameStateMachine::draw(sf::RenderTarget &app)
    {
        if (m_endState && m_endState->m_currentState)
            {
                drawState(m_endState, app);
            }
    }

void fe::gameStateMachine::postDraw()
    {
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
    }

fe::sceneGraph &fe::gameStateMachine::getSceneGraph()
    {
        return m_endState->m_currentState->getSceneGraph();
    }

const fe::baseGameState &fe::gameStateMachine::getCurrentState()
    {
        return *m_endState->m_currentState;
    }
