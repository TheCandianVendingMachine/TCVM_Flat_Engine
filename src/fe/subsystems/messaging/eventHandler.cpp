#include "fe/subsystems/messaging/eventHandler.hpp"

unsigned int fe::eventHandler::m_lastId = 1;

fe::eventHandler::eventHandler() : m_id(m_lastId++)
    {
    }

fe::eventHandler::eventHandler(unsigned int id) : m_id(id)
    {
    }

bool fe::eventHandler::operator==(const eventHandler &rhs)
    {
        return this == &rhs;
    }
