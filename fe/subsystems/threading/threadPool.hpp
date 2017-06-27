// threadPool.hpp
// a group of threads that takes in jobs, and works on them
#pragma once
#include <queue>
#include <thread>
#include <utility>
#include <chrono>
#include "threadJob.hpp"

namespace fe
    {
        template<unsigned int threadCount = 4>
        class threadPool
            {
                private:
                    struct threadObj
                        {
                            std::thread m_thread;
                            std::queue<threadJob*> m_jobs;
                            std::chrono::microseconds m_sleepTime;
                            unsigned int m_jobCount;

                            bool m_running;

                            void runJob(threadJob *newJob);
                            void update();

                        } m_pool[threadCount];

                public:
                    void startUp();

                    // adds a job to a thread for future calculations
                    void addJob(threadJob &job);
                    // blocking function that halts the program until the job is complete
                    void waitFor(threadJob &job);

                    void shutDown();

            };

        template<unsigned int threadCount>
        void threadPool<threadCount>::startUp()
            {
                for (unsigned int i = 0; i < 4; i++)
                    {
                        m_pool[i].m_running = true;
                        m_pool[i].m_sleepTime = std::chrono::microseconds(1);
                        m_pool[i].m_thread = std::thread(&threadPool::threadObj::update, &m_pool[i]);
                        m_pool[i].m_jobCount = 0;
                    }
            }

        // adds a job to a thread for future calculations
        template<unsigned int threadCount>
        void threadPool<threadCount>::addJob(threadJob &job)
            {
                if (!job.m_active) return;
                job.m_done = false;
                threadObj *selected = &m_pool[0];
                for (unsigned int i = 1; i < 4; i++)
                    {
                        if (m_pool[i].m_jobCount < selected->m_jobCount)
                            {
                                selected = &m_pool[i];
                            }
                    }

                selected->runJob(&job);
            }

        // blocking function that halts the program until the job is complete
        template<unsigned int threadCount>
        void threadPool<threadCount>::waitFor(threadJob &job)
            {
                while (!job.m_done && job.m_active) {}
            }

        template<unsigned int threadCount>
        void threadPool<threadCount>::shutDown()
            {
                for (unsigned int i = 0; i < threadCount; i++)
                    {
                        m_pool[i].m_running = false;
                    }

                for (unsigned int i = 0; i < threadCount; i++)
                    {
                        m_pool[i].m_thread.join();
                    }
            }

        template<unsigned int threadCount>
        void threadPool<threadCount>::threadObj::runJob(threadJob *newJob)
            {
                m_jobs.push(newJob);
                m_sleepTime = std::chrono::microseconds(0);
                m_jobCount++;
            }

        template<unsigned int threadCount>
        void threadPool<threadCount>::threadObj::update()
            {
                while (m_running)
                    {
                        // wait for a job
                        while (m_jobs.empty()) 
                            {
                                if (!m_running) return; 
                                std::this_thread::sleep_for(m_sleepTime); 
                            }

                        while (!m_jobs.back()->execute()) 
                            {
                                if (!m_running) return;
                            }

                        m_jobs.back()->m_done = true;
                        m_jobs.pop();
                        m_jobCount--;

                        if (m_jobCount <= 0)
                            {
                                m_sleepTime = std::chrono::microseconds(1);
                            }
                    }
            }
    }
