// inputManager.hpp
// Manages all inputs. Allows any class to see if a certain input was pressed
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "input.hpp"

#include <unordered_map>
#include <vector>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace fe
    {
        class inputManager : public fe::handleManager<input*>
            {
                private:
                    std::unordered_map<sf::Keyboard::Key, std::vector<Handle>> m_keyboardInputs;
                    std::unordered_map<sf::Mouse::Button, std::vector<Handle>> m_mouseInputs;

                    static inputManager *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void preUpdate();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API void handleKeyPress();

                    FLAT_ENGINE_API static inputManager &get();

                    FLAT_ENGINE_API unsigned int add(sf::Keyboard::Key key, input data);
                    FLAT_ENGINE_API unsigned int add(sf::Mouse::Button key, input data);

                    FLAT_ENGINE_API void setActive(sf::Keyboard::Key key, bool active);
                    FLAT_ENGINE_API void setActive(sf::Mouse::Button key, bool active);
                    FLAT_ENGINE_API void setActive(unsigned int handle, bool active);

                    FLAT_ENGINE_API virtual ~inputManager() {}

            };

    }