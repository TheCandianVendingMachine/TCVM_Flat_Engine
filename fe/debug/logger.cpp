#include "logger.hpp"
#include "../feAssert.hpp"
#include <SFML/System/Err.hpp>

fe::logger *fe::logger::m_instance = nullptr;

void fe::logger::startUp(const char *directory)
    {
        if (!m_instance)
            {
                m_instance = this;
                m_output.open(directory);
                assert(!m_output.fail() && "Error: Logger cannot be opened");
                sf::err().rdbuf(m_output.rdbuf());
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

