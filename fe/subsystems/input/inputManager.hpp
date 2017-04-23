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
                    std::unordered_map<std::string, inputBase*> m_inputs;

                    static inputManager *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API void handleKeyPress();

                    FLAT_ENGINE_API static inputManager &get();

                    FLAT_ENGINE_API void add(const char *id, input<sf::Keyboard::Key> input);
                    FLAT_ENGINE_API void add(const char *id, input<sf::Mouse::Button> input);

                    FLAT_ENGINE_API void remove(const char *id);

                    FLAT_ENGINE_API void setActive(const char *id, bool value);

                    FLAT_ENGINE_API virtual ~inputManager() {}

            };

    }