// animationActor.hpp
// holds a sf::VertexArray and anything relavent to animation
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../math/Vector2.hpp"
#include "../../../time/clock.hpp"

namespace sf
    {
        class VertexArray;
    }

namespace fe 
    {
        class animationActor
            {
                private:
                    sf::VertexArray *m_verticies;
                    fe::time m_lastCheckedTime;
                    fe::time m_pauseTime;
                    unsigned int m_animationFrameSpeed; // how long it takes for a single frame to iterate in milliseconds

                    unsigned int m_endFrame;
                    unsigned int m_startFrame;
                    unsigned int m_currentFrame;

                    bool m_play;

                public:
                    FLAT_ENGINE_API animationActor(sf::VertexArray *verticies);

                    FLAT_ENGINE_API void play(bool value);
                    FLAT_ENGINE_API bool isPlaying() const;

                    // If the actor needs to update its texture this will return true
                    FLAT_ENGINE_API bool needsUpdate(fe::time elapsedtime);

                    // Set how long it takes a frame to iterate in milliseconds
                    FLAT_ENGINE_API void setFrameSpeed(unsigned int animationSpeed);
                    // Get how long it takes a frame to iterate in milliseconds
                    FLAT_ENGINE_API unsigned int getFrameSpeed() const;

                    // Set the maximum frame that the animation can play through
                    FLAT_ENGINE_API void setEndFrame(unsigned int maxFrames);
                    // Get the maximum frame the animation can play through
                    FLAT_ENGINE_API unsigned int getEndFrame();

                    // Set the first frame which the animation will uses
                    FLAT_ENGINE_API void setStartFrame(unsigned int frame);
                    // Get the first frame which the animation will use
                    FLAT_ENGINE_API unsigned int getStartFrame();

                    // Set the current frame the animation is at
                    FLAT_ENGINE_API void setCurrentFrame(unsigned int frame);
                    // Get the current frame the animation is at
                    FLAT_ENGINE_API unsigned int getCurrentFrame() const;
                    // Iterate the current frame by the amount specified
                    FLAT_ENGINE_API void iterateFrame(int amount);

                    // Updates the verticies according to the texture
                    FLAT_ENGINE_API void updateVerticies(fe::Vector2<unsigned int> offset, fe::Vector2<unsigned int> size);

            };
    }