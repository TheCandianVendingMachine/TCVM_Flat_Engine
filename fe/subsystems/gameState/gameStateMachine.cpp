#include "gameStateMachine.hpp"
#include "../memory/memoryManager.hpp"
#include "../messaging/eventSender.hpp"
#include "../../engine.hpp"
#include "gameState.hpp"

fe::gameStateMachine *fe::gameStateMachine::m_instance = nullptr;

void fe::gameStateMachine::startUp()
    {
        if (!m_instance) 
            {
                auto memBuf = FE_ALLOC_STACK("StateBuffer", 16_KiB);
                m_stateAllocater.startUp(static_cast<char*>(memBuf), 16_KiB);

                m_stateMarker = fe::memoryManager::get().getStackAllocater().getMarker();
                m_instance = this;

                m_pop = false;
                m_currentState = nullptr;
                m_nextState = nullptr;

                m_update = true;
            }
    }

void fe::gameStateMachine::shutDown()
    {
        pop();
    }

fe::gameStateMachine &fe::gameStateMachine::get()
    {
        return *m_instance;
    }

void fe::gameStateMachine::push(baseGameState *newState)
    {
        auto listBuf = FE_ALLOC_STACK("StateListAlloc", sizeof(stateList));
        m_endState->m_head = new(listBuf) stateList();

        m_endState = m_endState->m_head;

        m_endState->m_currentState = newState;
        m_endState->m_currentState->init();
        m_update = false;
    }

void fe::gameStateMachine::pop()
    {
        if (m_endState)
            {
                m_endState->m_currentState->deinit();
                delete m_endState->m_currentState;

                m_endState = m_endState->m_tail;

                delete m_endState->m_head;
                m_endState->m_head = nullptr;

                FE_FREE_STACK("GameStateMachine", m_endState->m_offset);
            }
        m_stateMarker = fe::memoryManager::get().getStackAllocater().getMarker();
    }

void fe::gameStateMachine::queuePop()
    {
        m_pop = true;
    }

void fe::gameStateMachine::handleEvents(const sf::Event &event)
    {
        if (m_currentState)
            {
                m_currentState->handleEvents(event);
            }
    }

void fe::gameStateMachine::preUpdate()
    {
        if (m_pop)
            {
                pop();
                m_pop = false;
            }

        if (m_nextState)
            {
                push(m_nextState);
                m_nextState = nullptr;
            }

        if (m_currentState)
            {
                m_currentState->preUpdate();
            }
    }

void fe::gameStateMachine::update(float deltaTime)
    {
        if (m_currentState && m_update)
            {
                m_currentState->update(deltaTime);
            }
    }

void fe::gameStateMachine::postUpdate()
    {
        if (m_currentState)
            {
                m_currentState->postUpdate();
                m_currentState->postUpdateDefined();
            }

        if (!m_update)
            {
                m_update = true;
            }
    }

void fe::gameStateMachine::preDraw()
    {
        if (m_currentState)
            {
                m_currentState->preDraw();
            }
    }

void fe::gameStateMachine::draw(sf::RenderTarget &app)
    {
        if (m_currentState)
            {
                m_currentState->draw(app);
            }
    }

void fe::gameStateMachine::postDraw()
    {
        if (m_currentState)
            {
                m_currentState->postDraw();
            }
    }

fe::sceneGraph &fe::gameStateMachine::getSceneGraph()
    {
        return m_currentState->getSceneGraph();
    }

const fe::baseGameState &fe::gameStateMachine::getCurrentState()
    {
        return *m_currentState;
    }
