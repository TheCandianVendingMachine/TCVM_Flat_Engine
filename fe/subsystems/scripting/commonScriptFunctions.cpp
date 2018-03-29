#include "commonScriptFunctions.hpp"
#include "../../engine.hpp"
#include "../input/inputManager.hpp"

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
