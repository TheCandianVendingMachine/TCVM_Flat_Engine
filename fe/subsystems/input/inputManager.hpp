// inputManager.hpp
// Manages all inputs. Allows any class to see if a certain input was pressed
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "input.hpp"

#include <unordered_map>
#include <string>

namespace fe
    {
        class inputManager
            {
                private:
                    std::unordered_map<std::string, input<sf::Keyboard::Key>> m_keyboardInputs;
                    std::unordered_map<std::string, input<sf::Mouse::Button>> m_mouseInputs;

                    static inputManager *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);

                    FLAT_ENGINE_API static inputManager &get();

                    FLAT_ENGINE_API void add(const std::string &id, input<sf::Keyboard::Key> input);
                    FLAT_ENGINE_API void add(const std::string &id, input<sf::Mouse::Button> input);

                    FLAT_ENGINE_API bool isKeyPressed(const std::string &id);
                    FLAT_ENGINE_API bool isButtonPressed(const std::string &id);

                    FLAT_ENGINE_API bool keyEventPress(const std::string &id);
                    FLAT_ENGINE_API bool buttonEventPress(const std::string &id);

            };
    }