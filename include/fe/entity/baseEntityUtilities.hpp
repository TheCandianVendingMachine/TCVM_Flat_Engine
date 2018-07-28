// baseEntityUtilities.hpp
// utilities for entities to use in order to move or other such things
#pragma once
#define FLAT_ENGINE_EXPORT
#include <fe/flatEngineExport.hpp>
#include <fe/math/Vector2.hpp>

namespace fe 
    {
        class baseEntity;
        class baseEntityUtilities 
            {
                private:
                    baseEntity *const m_baseEntity;

                    float m_slowdownDistance;
                    float m_completionRadius;
                    fe::Vector2d m_targetPosition;
                    bool m_targeted;

                    bool m_arrived;

                public:
                    FLAT_ENGINE_API baseEntityUtilities(baseEntity *entity);

                    FLAT_ENGINE_API void setSlowdownDistance(float distance);

                    FLAT_ENGINE_API void moveToPosition(const fe::Vector2d position);
                    FLAT_ENGINE_API void moveToPosition(float x, float y);

                    FLAT_ENGINE_API float getSlowdownDistance() const;

                    FLAT_ENGINE_API void setCompletionRadius(float radius);
                    FLAT_ENGINE_API float getCompletionRadius() const;

                    FLAT_ENGINE_API void update();

                    FLAT_ENGINE_API bool getArrived() const;
                    FLAT_ENGINE_API bool isTargeted() const;

            };
    }
