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
        class inputManager : protected fe::handleManager<input*, 0>
            {
                private:
                    std::unordered_map<sf::Event::EventType, std::vector<fe::Handle>> m_eventInputs;
                    std::vector<fe::Handle> m_realtimeInputs;

                    std::unordered_map<sf::Keyboard::Key, bool> m_realtimeKeyPressed;

                    static inputManager *m_instance;

                    FLAT_ENGINE_API bool correctInputEvent(sf::Keyboard::Key key, const sf::Event &event);
                    FLAT_ENGINE_API bool correctInputEvent(sf::Mouse::Button button, const sf::Event &event);
                    FLAT_ENGINE_API bool correctInputEvent(sf::Mouse::Wheel wheel, const sf::Event &event);

                    FLAT_ENGINE_API bool correctInputEvent(sf::Keyboard::Key key);
                    FLAT_ENGINE_API bool correctInputEvent(sf::Mouse::Button button);

                    FLAT_ENGINE_API sf::Event::EventType getEvent(input *input, sf::Keyboard::Key key);
                    FLAT_ENGINE_API sf::Event::EventType getEvent(input *input, sf::Mouse::Button button);
                    FLAT_ENGINE_API sf::Event::EventType getEvent(input *input, sf::Mouse::Wheel wheel);

                    FLAT_ENGINE_API void setActive(fe::input *input, bool keyIsGood, bool active);

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void preUpdate();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API void handleKeyPress();

                    FLAT_ENGINE_API[[deprecated]] static inputManager &get();

                    FLAT_ENGINE_API fe::Handle add(sf::Keyboard::Key key, input data);
                    FLAT_ENGINE_API fe::Handle add(sf::Mouse::Button button, input data);
                    FLAT_ENGINE_API fe::Handle add(sf::Mouse::Wheel  wheel, input data);
                    FLAT_ENGINE_API void remove(fe::Handle handle);

                    FLAT_ENGINE_API void setActive(sf::Keyboard::Key key, bool active);
                    FLAT_ENGINE_API void setActive(sf::Mouse::Button button, bool active);
                    FLAT_ENGINE_API void setActive(sf::Mouse::Wheel wheel, bool active);
                    FLAT_ENGINE_API void setActive(fe::Handle handle, bool active);

                    FLAT_ENGINE_API bool isKeyPressed(sf::Keyboard::Key key) const;

                    FLAT_ENGINE_API ~inputManager() {}

            };

    }