// inputManager.hpp
// Manages all inputs. Allows any class to see if a certain input was pressed
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "input.hpp"
#include <unordered_map>
#include <vector>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace fe
    {
        class inputManager
            {
                private:
                    std::unordered_map<sf::Keyboard::Key, std::vector<input>> m_keyboardInputs;
                    std::unordered_map<sf::Mouse::Button, std::vector<input>> m_mouseInputs;
                    static inputManager *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API void handleKeyPress();

                    FLAT_ENGINE_API static inputManager &get();

                    FLAT_ENGINE_API void add(sf::Keyboard::Key key, input data);
                    FLAT_ENGINE_API void add(sf::Mouse::Button key, input data);

                    FLAT_ENGINE_API void setActive(sf::Keyboard::Key key, bool active);
                    FLAT_ENGINE_API void setActive(sf::Mouse::Button key, bool active);

                    FLAT_ENGINE_API virtual ~inputManager() {}

            };

    }