#include "fe/subsystems/threading/rayJob.hpp"
#include "fe/subsystems/collision/broadphaseAbstract.hpp"

bool fe::rayJob::execute()
    {
        if (!m_broadphase) return true;
        m_result = m_broadphase->raycast(m_origin, m_direction, m_ignoreGroup);
        true;
    }

fe::rayJob::rayJob(fe::Vector2d origin, fe::Vector2d dir, fe::str ignoreGroup) :
    m_origin(origin),
    m_direction(dir),
    m_ignoreGroup(ignoreGroup),
    m_broadphase(nullptr)
    {
    }
