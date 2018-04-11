// commonScriptFunctions.hpp
// script functions that require a layer of abstraction in order to be utilized properly
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <sol.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace fe
    {
        class scriptObject;

        FLAT_ENGINE_API bool isInputPressed(int input);
        FLAT_ENGINE_API bool isKeyPressed(sf::Keyboard::Key input);
        FLAT_ENGINE_API bool isMousePressed(sf::Mouse::Button input);
        FLAT_ENGINE_API void setCameraPosition(float x, float y);
        FLAT_ENGINE_API void sendEvent(const char *eventID, sol::variadic_args args);
        FLAT_ENGINE_API void breakToDebugger();
    }
