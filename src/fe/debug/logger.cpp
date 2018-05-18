#include "fe/debug/logger.hpp"
#include "fe/feAssert.hpp"
#include "fe/subsystems/threading/threadJob.hpp"
#include "fe/subsystems/threading/threadPool.hpp"
#include "fe/engine.hpp"
#include <SFML/System/Err.hpp>

fe::logger *fe::logger::m_instance = nullptr;

fe::logger::stringStreamWrapper &fe::logger::getCurrentFileBuffer()
    {
        return *m_currentFileBuffer;
    }

fe::logger::stringStreamWrapper &fe::logger::getCurrentConsoleBuffer()
    {
        return *m_currentConsoleBuffer;
    }

bool fe::logger::printLogs()
    {
        m_fileBufferLock.lock();
        std::stringstream *fileBuffer = &m_nextFileBuffer->m_buffer;
        if (m_nextFileBuffer->m_ready)
            {
                m_output << fileBuffer->rdbuf();
                m_output.flush();
                m_nextFileBuffer->m_ready = false;
            }
        m_fileBufferLock.unlock();

        m_consoleBufferLock.lock();
        std::stringstream *consoleBuffer = &m_nextConsoleBuffer->m_buffer;
        if (m_nextConsoleBuffer->m_ready)
            {
                std::cout << consoleBuffer->rdbuf();
                m_nextConsoleBuffer->m_ready = false;
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
        stringStreamWrapper *currentBufferTemp = m_currentFileBuffer;
        m_currentFileBuffer = m_nextFileBuffer;
        m_nextFileBuffer = currentBufferTemp;
        m_fileBufferLock.unlock();
    }

void fe::logger::swapConsoleBuffer()
    {
        m_consoleBufferLock.lock();
        stringStreamWrapper *currentBufferTemp = m_currentConsoleBuffer;
        m_currentConsoleBuffer = m_nextConsoleBuffer;
        m_nextConsoleBuffer = currentBufferTemp;
        m_consoleBufferLock.unlock();
    }
