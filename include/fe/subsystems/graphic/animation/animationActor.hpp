// animationActor.hpp
// Iterates through the animation. Requires you to inject a fe::renderObject to interate
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/time/time.hpp"
#include "fe/typeDefines.hpp"

namespace fe 
    {
        struct renderObject;
        class animationTexture;
        class animationActor
            {
                private:
                    fe::renderObject *const m_actorVerticies;
                    fe::time m_lastCheckedTime;
                    fe::time m_pauseTime;

                    fe::str m_currentAnimationSequence;
                    unsigned int m_currentSequenceFrame;

                    fe::Vector2<unsigned int> m_currentFrame;

                    bool m_play;            // whether or not the sequence is paused
                    bool m_needsUpdate;     // if the sequence needs to iterate current texture
                    bool m_playingSequence; // whether or not a sequence has been selected
                    bool m_doneSequence;    // if the sequence has concluded

                public:
                    FLAT_ENGINE_API animationActor(fe::renderObject *const actor);

                    FLAT_ENGINE_API void play(bool value);
                    FLAT_ENGINE_API void stop();
                    FLAT_ENGINE_API bool isPlaying() const;

                    // If the actor needs to update its texture this will return true
                    FLAT_ENGINE_API bool needsUpdate() const;
                    FLAT_ENGINE_API void update(const fe::animationTexture &texture, fe::time elapsedTime);

                    // Set how long it takes a frame to iterate in milliseconds
                    FLAT_ENGINE_API void setFrameSpeed(unsigned int animationSpeed);
                    // Get how long it takes a frame to iterate in milliseconds
                    FLAT_ENGINE_API unsigned int getFrameSpeed() const;

                    // Set the current frame the animation is at
                    FLAT_ENGINE_API void setCurrentFrame(unsigned int x, unsigned int y);
                    // Get the current frame the animation is at
                    FLAT_ENGINE_API fe::Vector2<unsigned int> getCurrentFrame() const;

                    // Updates the verticies according to the texture
                    FLAT_ENGINE_API void updateVerticies(fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> size);
                    
                    FLAT_ENGINE_API void playSequence(fe::str sequence);

            };
    }