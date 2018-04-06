#include "fe/subsystems/scripting/commonScriptFunctions.hpp"
#include "fe/subsystems/input/inputManager.hpp"
#include "fe/subsystems/gameState/gameState.hpp"
#include "fe/engine.hpp"
#include <intrin.h>

bool fe::isInputPressed(int input)
    {
        if (input >= sf::Keyboard::Key::KeyCount)
            {
                return isMousePressed(sf::Mouse::Button(input - sf::Keyboard::Key::KeyCount));
            }
        return isKeyPressed(sf::Keyboard::Key(input));
    }

bool fe::isKeyPressed(sf::Keyboard::Key input)
    {
        return sf::Keyboard::isKeyPressed(input);
    }

bool fe::isMousePressed(sf::Mouse::Button input)
    {
        return sf::Mouse::isButtonPressed(input);
    }

void fe::setCameraPosition(float x, float y)
    {
        fe::engine::get().getCurrentState().getCamera().setPosition(x, y);
    }

void fe::breakToDebugger()
    {
        __debugbreak();
    }
