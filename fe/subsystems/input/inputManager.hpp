// inputManager.hpp
// Manages all inputs. Allows any class to see if a certain input was pressed
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "input.hpp"
#include <vector>

namespace fe
    {
        class inputManager
            {
                private:
                    std::vector<input<sf::Keyboard::Key>> m_keyboardInputs;
                    std::vector<input<sf::Mouse::Button>> m_mouseInputs;

                    static inputManager *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API void handleKeyPress();

                    FLAT_ENGINE_API static inputManager &get();

                    FLAT_ENGINE_API void add(input<sf::Keyboard::Key> input);
                    FLAT_ENGINE_API void add(input<sf::Mouse::Button> input);

                    FLAT_ENGINE_API virtual ~inputManager() {}

            };
    }