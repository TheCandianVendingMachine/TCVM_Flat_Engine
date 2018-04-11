#include "fe/subsystems/scripting/commonScriptFunctions.hpp"
#include "fe/subsystems/input/inputManager.hpp"
#include "fe/subsystems/gameState/gameState.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/subsystems/messaging/gameEvent.hpp"
#include "fe/objectManagement/str.hpp"
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
        return fe::engine::get().getInputManager().isKeyPressed(input);
    }

bool fe::isMousePressed(sf::Mouse::Button input)
    {
        return sf::Mouse::isButtonPressed(input);
    }

void fe::setCameraPosition(float x, float y)
    {
        fe::engine::get().getCurrentState().getCamera().setPosition(x, y);
    }

void fe::sendEvent(const char *eventID, sol::variadic_args args)
    {
        fe::gameEvent event(FE_STR(eventID), args.size());
        sol::object objs[fe::gameEvent::ARGUMENT_COUNT];
        for (unsigned int i = 0; i < args.size(); i++)
            {
                objs[i] = args[i].as<sol::object>();
                switch (args[i].get_type())
                    {
                        case sol::type::boolean:
                            event.args[i].argType = fe::gameEventArgument::type::TYPE_BOOL;
                            event.args[i].arg.TYPE_BOOL = args[i].as<bool>();
                            break;
                        case sol::type::number:
                            event.args[i].argType = fe::gameEventArgument::type::TYPE_FLOAT;
                            event.args[i].arg.TYPE_FLOAT = args[i].as<float>();
                            break;
                        case sol::type::userdata:
                            event.args[i].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                            event.args[i].arg.TYPE_VOIDP = &objs[i];
                            break;
                        default:
                            FE_LOG_WARNING("Argument Type Not Supported:", static_cast<int>(args[i].get_type()));
                            break;
                    }
            }
        fe::engine::get().getEventSender().send(event, event.eventType);
    }

void fe::breakToDebugger()
    {
        __debugbreak();
    }
