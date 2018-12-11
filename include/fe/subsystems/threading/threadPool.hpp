// threadPool.hpp
// a group of threads that takes in jobs, and works on them
#pragma once
#include <deque>
#include <queue>
#include <thread>
#include <utility>
#include <chrono>
#include <mutex>
#include "threadJob.hpp"

namespace fe
    {
        template<unsigned int threadCount = 4, unsigned int hertz = 100>
        class threadPool
            {
                private:
                    struct threadObj
                        {
                            std::thread m_thread;
                            std::mutex m_jobLock;
                            std::deque<threadJob*> m_jobs;
                            std::queue<threadJob*> m_queuedJobs;
                            unsigned int m_jobCount;
                            std::chrono::microseconds m_tickRate;

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

        template<unsigned int threadCount, unsigned int hertz>
        void threadPool<threadCount, hertz>::startUp()
            {
                std::chrono::microseconds tickRate(1000000 / hertz);
                for (unsigned int i = 0; i < threadCount; i++)
                    {
                        m_pool[i].m_running = true;
                        m_pool[i].m_thread = std::thread(&threadPool::threadObj::update, &m_pool[i]);
                        m_pool[i].m_jobCount = 0;
                        m_pool[i].m_tickRate = tickRate;
                    }
            }

        // adds a job to a thread for future calculations
        template<unsigned int threadCount, unsigned int hertz>
        void threadPool<threadCount, hertz>::addJob(threadJob &job)
            {
                if (job.m_active) return;
                job.m_active = true;
                threadObj *selected = &m_pool[0];
                for (unsigned int i = 1; i < threadCount; i++)
                    {
                        if (m_pool[i].m_jobCount < selected->m_jobCount)
                            {
                                selected = &m_pool[i];
                            }
                    }

                job.m_done = false;
                job.m_started = true;
                selected->runJob(&job);
            }

        // blocking function that halts the program until the job is complete
        template<unsigned int threadCount, unsigned int hertz>
        void threadPool<threadCount, hertz>::waitFor(threadJob &job)
            {
                while (!job.isDone()) {}
                job.m_started = false;
            }

        template<unsigned int threadCount, unsigned int hertz>
        void threadPool<threadCount, hertz>::shutDown()
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

        template<unsigned int threadCount, unsigned int hertz>
        void threadPool<threadCount, hertz>::threadObj::runJob(threadJob *newJob)
            {
                m_jobLock.lock();
                m_queuedJobs.push(newJob);
                m_jobLock.unlock();
                m_jobCount++;
            }

        template<unsigned int threadCount, unsigned int hertz>
        void threadPool<threadCount, hertz>::threadObj::update()
            {
                while (m_running)
                    {
                        if (!m_jobs.empty() || !m_queuedJobs.empty())
                            {
                                if (!m_queuedJobs.empty())
                                    {
                                        m_jobLock.lock();
                                        while (!m_queuedJobs.empty())
                                            {
                                                m_jobs.push_back(m_queuedJobs.front());
                                                m_queuedJobs.pop();
                                            }
                                        m_jobLock.unlock();
                                    }

                                for (auto it = m_jobs.begin(); it != m_jobs.end();)
                                    {
                                        auto job = (*it);
                                        if (job->execute() || job->isDone())
                                            {
                                                job->m_active = false;
                                                job->m_done = true;
                                                it = m_jobs.erase(it);
                                            }
                                        else
                                            {
                                                ++it;
                                            }
                                    }
                            }

                        std::this_thread::sleep_for(m_tickRate);
                    }
            }
    }
