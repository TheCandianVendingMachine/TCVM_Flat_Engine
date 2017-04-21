#include "gameStateMachine.hpp"
#include "../../engine.hpp"
#include "../memory/memoryManager.hpp"
#include "gameState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

fe::gameStateMachine *fe::gameStateMachine::m_instance = nullptr;

void fe::gameStateMachine::startUp()
    {
        if (!m_instance) 
            {
                auto memBuf = FE_ALLOC_DIRECT("StateBuffer", 16_KiB);
                m_stateAllocater.startUp(static_cast<char*>(memBuf), 16_KiB);

                m_stateMarker = fe::memoryManager::get().getStackAllocater().getMarker();
                m_instance = this;

                m_pop = false;
                m_currentState = nullptr;
                m_nextState = nullptr;
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
        m_currentState = newState;
        m_currentState->init();
    }

void fe::gameStateMachine::pop()
    {
        // we clear all event handlers so we don't call on a nullptr by accident
        //fe::engine::get().getEventSender()->clear();
        if (m_currentState)
            {
                m_currentState->deinit();

                // This must be in this scope because if we free to the marker and there isnt a state, the marker will be pointing to an area made
                // AFTER CONSTRUCTION. This will cause a ton of errors
                FE_FREE_STACK("StateMachine", m_stateMarker);
            }
        m_stateMarker = fe::memoryManager::get().getStackAllocater().getMarker();
        m_currentState = nullptr;
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
        if (m_currentState)
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
    }

void fe::gameStateMachine::preDraw()
    {
        if (m_currentState)
            {
                m_currentState->preDraw();
            }
    }

void fe::gameStateMachine::draw(sf::RenderWindow &app)
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
