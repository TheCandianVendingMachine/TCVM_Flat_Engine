// baseGameScreen.hpp
// A sub-state for the current "screen" of the game
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"

namespace fe
    {
        class baseGameState;
        class baseGameScreen
            {
                private:
                    baseGameState *m_gameState;

                protected:
                    FLAT_ENGINE_API baseGameState *const getState() const;

                public:
                    FLAT_ENGINE_API void setState(baseGameState *state);

                    virtual void init() {}
                    virtual void preUpdate() {}
                    virtual void postUpdate() {}
                    virtual void deinit() {}

            };
    }
