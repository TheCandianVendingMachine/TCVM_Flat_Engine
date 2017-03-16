// inputManager.hpp
// Manages all inputs. Allows any class to see if a certain input was pressed
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "input.hpp"
#include "../memory/memoryManager.hpp"
#include <vector>

namespace fe
    {
        class inputManager
            {
                private:
                    std::vector<inputBase*> m_inputs;
                    static inputManager *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void handleEvents(const sf::Event &event);
                    FLAT_ENGINE_API void handleKeyPress();

                    FLAT_ENGINE_API static inputManager &get();

                    FLAT_ENGINE_API void add(input<sf::Keyboard::Key> input);
                    FLAT_ENGINE_API void add(input<sf::Mouse::Button> input);

                    template<typename Obj>
                    void add(input<sf::Keyboard::Key, Obj*> input);
                    template<typename Obj>
                    void add(input<sf::Mouse::Button, Obj*> input);

                    FLAT_ENGINE_API virtual ~inputManager() {}

            };

        template<typename Obj>
        void fe::inputManager::add(input<sf::Keyboard::Key, Obj*> input)
            {
                void *mem = FE_ALLOC_STACK("InputManager", sizeof(fe::input<sf::Keyboard::Key, Obj*>));
                fe::input<sf::Keyboard::Key, Obj*> *inputAlloc = new(mem) fe::input<sf::Keyboard::Key, Obj*>;

                *inputAlloc = input;
                m_inputs.push_back(inputAlloc);
            }

        template<typename Obj>
        void fe::inputManager::add(input<sf::Mouse::Button, Obj*> input)
            {
                void *mem = FE_ALLOC_STACK("InputManager", sizeof(fe::input<sf::Mouse::Button, Obj*>));
                fe::input<sf::Mouse::Button, Obj*> *inputAlloc = new(mem) fe::input<sf::Mouse::Button, Obj*>;

                *inputAlloc = input;
                m_inputs.push_back(inputAlloc);
            }

    }