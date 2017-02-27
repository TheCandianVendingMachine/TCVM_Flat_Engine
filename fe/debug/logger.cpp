#include "logger.hpp"
#include "../feAssert.hpp"

fe::logger *fe::logger::m_instance = nullptr;

void fe::logger::startUp(const char *directory)
    {
        if (!m_instance)
            {
                m_instance = this;
                m_output.open(directory);
                FE_ASSERT(m_output.is_open(), "Error: Logger cannot be opened");
            }
    }

void fe::logger::shutDown()
    {
        m_output.close();
    }

fe::logger &fe::logger::get()
    {
        return *m_instance;
    }
