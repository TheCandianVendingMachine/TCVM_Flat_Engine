#include "logger.hpp"
#include "../feAssert.hpp"
#include "../subsystems/threading/threadJob.hpp"
#include "../subsystems/threading/threadPool.hpp"
#include "../engine.hpp"
#include <SFML/System/Err.hpp>

fe::logger *fe::logger::m_instance = nullptr;

std::stringstream &fe::logger::getCurrentFileBuffer()
    {
        return *m_currentFileBuffer;
    }

std::stringstream &fe::logger::getCurrentConsoleBuffer()
    {
        return *m_currentConsoleBuffer;
    }

bool fe::logger::printLogs()
    {
        m_fileBufferLock.lock();
        std::stringstream *fileBuffer = m_nextFileBuffer;
        if (fileBuffer->rdbuf()->in_avail() > 0)
            {
                m_output << fileBuffer->rdbuf();
                m_output.flush();
            }
        m_fileBufferLock.unlock();

        m_consoleBufferLock.lock();
        std::stringstream *consoleBuffer = m_nextConsoleBuffer;
        if (consoleBuffer->rdbuf()->in_avail() > 0)
            {
                std::cout << consoleBuffer->rdbuf();
            }
        m_consoleBufferLock.unlock();

        return !m_running;
    }

fe::logger::logger() :
    m_printJob([]()->bool { return false; }),
    m_running(false),
    m_currentConsoleBuffer(nullptr),
    m_currentFileBuffer(nullptr),
    m_nextConsoleBuffer(nullptr),
    m_nextFileBuffer(nullptr)
    {
    }

void fe::logger::startUp(const char *directory)
    {
        if (!m_instance)
            {
                m_instance = this;
                m_running = true;
                m_output.open(directory);
                assert(!m_output.fail() && "Error: Logger cannot be opened");
                sf::err().rdbuf(m_output.rdbuf());

                m_currentConsoleBuffer = &m_consoleBuffer[0];
                m_nextConsoleBuffer = &m_consoleBuffer[1];

                m_currentFileBuffer = &m_fileBuffer[0];
                m_nextFileBuffer = &m_fileBuffer[1];

                m_printJob.m_function = std::bind(&fe::logger::printLogs, m_instance);
                fe::engine::get().getThreadPool().addJob(m_printJob);
            }
    }

void fe::logger::shutDown()
    {
        m_running = false;
        fe::engine::get().getThreadPool().waitFor(m_printJob);
        m_output.close();
    }

fe::logger &fe::logger::get()
    {
        return *m_instance;
    }

void fe::logger::swapFileBuffer()
    {
        m_fileBufferLock.lock();
        std::stringstream *currentBufferTemp = m_currentFileBuffer;
        m_currentFileBuffer = m_nextFileBuffer;
        m_nextFileBuffer = currentBufferTemp;
        m_fileBufferLock.unlock();
    }

void fe::logger::swapConsoleBuffer()
    {
        m_consoleBufferLock.lock();
        std::stringstream *currentBufferTemp = m_currentConsoleBuffer;
        m_currentConsoleBuffer = m_nextConsoleBuffer;
        m_nextConsoleBuffer = currentBufferTemp;
        m_consoleBufferLock.unlock();
    }
