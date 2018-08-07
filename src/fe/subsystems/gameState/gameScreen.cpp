#include "fe/subsystems/gameState/gameScreen.hpp"

fe::baseGameState *const fe::baseGameScreen::getState() const
    {
        return m_gameState;
    }

void fe::baseGameScreen::setState(baseGameState *state)
    {
        m_gameState = state;
    }
