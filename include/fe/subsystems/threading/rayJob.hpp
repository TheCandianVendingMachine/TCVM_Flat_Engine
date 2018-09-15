// rayJob.hpp
// A raycast request
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/threading/threadJob.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/objectManagement/str.hpp"
#include "fe/subsystems/physics/collision/raycastResult.hpp"

namespace fe
    {
        class broadphaseAbstract;
        struct rayJob : public fe::threadJob
            {
                const fe::Vector2d m_origin;
                const fe::Vector2d m_direction;
                const fe::str m_ignoreGroup;
                fe::broadphaseAbstract *m_broadphase;
                fe::raycastResult m_result;

                FLAT_ENGINE_API bool execute();
                FLAT_ENGINE_API rayJob(fe::Vector2d origin, fe::Vector2d dir, fe::str ignoreGroup);
            };
    }
