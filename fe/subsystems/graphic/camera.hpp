// camera.hpp
// A camera in the game world
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <SFML/Graphics/View.hpp>
#include "../../math/Vector2.hpp"

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class camera
            {
                private:
                    sf::View m_view;

                    fe::Vector2d m_position;
                    fe::Vector2d m_size;

                    fe::Vector2d m_acceleration;
                    fe::Vector2d m_velocity;
                    float m_maxSpeed;

                    float m_slowdownDistance;
                    fe::Vector2d m_targetPosition;
                    bool m_targeted;

                public:
                    FLAT_ENGINE_API camera();

                    FLAT_ENGINE_API void setSize(const fe::Vector2d size);
                    FLAT_ENGINE_API void setSize(float x, float y);

                    FLAT_ENGINE_API void setPosition(const fe::Vector2d position);
                    FLAT_ENGINE_API void setPosition(float x, float y);

                    FLAT_ENGINE_API void setVelocity(const fe::Vector2d velocity);
                    FLAT_ENGINE_API void setVelocity(float x, float y);

                    FLAT_ENGINE_API void setAcceleration(const fe::Vector2d acceleration);
                    FLAT_ENGINE_API void setAcceleration(float x, float y);

                    FLAT_ENGINE_API void setMaxSpeed(float speed);
                    FLAT_ENGINE_API void setSlowdownDistance(float distance);

                    FLAT_ENGINE_API void moveToPosition(const fe::Vector2d position);
                    FLAT_ENGINE_API void moveToPosition(float x, float y);

                    FLAT_ENGINE_API void move(const fe::Vector2d position);
                    FLAT_ENGINE_API void move(float x, float y);

                    FLAT_ENGINE_API fe::Vector2d getSize() const;
                    FLAT_ENGINE_API fe::Vector2d getPosition() const;
                    FLAT_ENGINE_API fe::Vector2d getAcceleration() const;
                    FLAT_ENGINE_API fe::Vector2d getVelocity() const;
                    FLAT_ENGINE_API float getMaxSpeed() const;
                    FLAT_ENGINE_API float getSlowdownDistance() const;

                    FLAT_ENGINE_API void updateCamera(float dt);
                    FLAT_ENGINE_API const sf::View &getView();

                    FLAT_ENGINE_API camera &operator=(const camera &rhs);
            };
    }