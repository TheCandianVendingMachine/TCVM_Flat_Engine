#include "fe/subsystems/threading/threadJob.hpp"

bool fe::threadJob::isDone() const
    {
        return m_done || !m_active;
    }
