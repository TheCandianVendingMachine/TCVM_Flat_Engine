#include "fe/debug/profilerLogger.hpp"
#include "fe/objectManagement/str.hpp"
#include "fe/time/clock.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/memory/memoryManager.hpp"
#include <algorithm>

fe::profilerLogger *fe::profilerLogger::m_instance = nullptr;

void fe::profilerLogger::print(std::ostream &out, profileData &data, const char *prefix)
    {
    #if !FE_PROFILE_PRINT_ZEROS
        if (data.m_avgTime.asMicroseconds() > 0)
    #endif
        {
            out <<          prefix << "Results:"            << " {"
                   "\n" <<  prefix << "\tCalls per frame: " << data.m_calls <<
                   "\n" <<  prefix << "\tMicroseconds: "    << data.m_avgTime.asMicroseconds() <<
                   "\n" <<  prefix << "\tMilliseconds: "    << data.m_avgTime.asMilliseconds() <<
                   "\n" <<  prefix << "\tSeconds: "         << data.m_avgTime.asSeconds() <<
                   "\n" <<  prefix << "\tFPS: "             << 1 / data.m_avgTime.asSeconds() << "\n" << prefix << "}\n";
        }
    }

void fe::profilerLogger::print(std::ostream &out, profileGroup &group, const char *prefix)
    {
        char newPrefix[16];
        std::strcpy(newPrefix, prefix);
        std::strcat(newPrefix, "\t");

        group.m_data->m_avgTime = fe::time();
        for (unsigned int i = 0; i < group.m_data->m_calls; i++)
            {
                group.m_data->m_avgTime += group.m_data->m_time[i];
            }
        if (group.m_data->m_calls > 0) 
            {
                group.m_data->m_avgTime /= group.m_data->m_calls;
            }
        out << prefix << group.m_name << " {\n";

        print(out, *group.m_data, newPrefix);

        for (auto &child : group.m_children)
            {
                for (unsigned int i = 0; i < group.m_data->m_calls; i++)
                    {
                        group.m_data->m_avgTime += group.m_data->m_time[i];
                    }
                if (group.m_data->m_calls > 0) 
                    {
                        group.m_data->m_avgTime /= group.m_data->m_calls;
                    }
            }
        std::sort(group.m_children.begin(), group.m_children.end(), [](const profileGroup *lhs, const profileGroup *rhs) -> bool { return *lhs > *rhs; });

        for (auto &child : group.m_children)
            {
                print(out, *child, newPrefix);
            }

        group.m_data->m_avgTime = fe::time();
        group.m_children.clear();
        out << prefix << "}\n\n";
    }

void fe::profilerLogger::startUp()
    {
        if (!m_instance) 
            {
                m_instance = this;
                const char *name = "profile_results";
                m_allProfileGroups[FE_STR(name)] = m_profileGroup[m_groupsCreated++].init(name);
                m_profileGroupStack.push(m_allProfileGroups[FE_STR(name)]);
                m_baseGroup = m_profileGroupStack.top();

                for (unsigned int i = 0; i < FE_MAX_PROFILER_PROFILES; i++)
                    {
                        m_unallocatedProfiles.insert(i);
                    }
            }
    }

void fe::profilerLogger::shutDown()
    {
        for (unsigned int i = 0; i < FE_MAX_PROFILER_PROFILES; i++)
            {
                m_allProfiles[i].shutDown();
                m_profileGroup[i].shutDown();
                m_unallocatedProfiles.clear();
            }
        m_instance = nullptr;
    }

fe::profilerLogger &fe::profilerLogger::get()
    {
        return *m_instance;
    }

void fe::profilerLogger::startProfile(const char *profile)
    {
        if (m_allProfileGroups.find(FE_STR(profile)) == m_allProfileGroups.end())
            {
                m_allProfileGroups[FE_STR(profile)] = m_profileGroup[m_groupsCreated++].init(profile);
                m_allProfileGroups[FE_STR(profile)]->m_data = &m_profileData[m_profilesCreated++];
            }

        profileGroup *top = m_profileGroupStack.top();

        if (std::find(top->m_children.begin(), top->m_children.end(), m_allProfileGroups[FE_STR(profile)]) == top->m_children.end())
            {
                top->m_children.push_back(m_allProfileGroups[FE_STR(profile)]);
            }
        m_profileGroupStack.push(m_allProfileGroups[FE_STR(profile)]);
    }

void fe::profilerLogger::endProfile(const char *profile, fe::time time)
    {
        profileGroup &group = *m_allProfileGroups[FE_STR(profile)];
        if (group.m_data->m_calls < FE_PROFILER_AVERAGE_MAX)
            {
                group.m_data->m_time[group.m_data->m_calls] = time;
                group.m_data->m_calls++;
            }
        group.m_data->m_totalCalls++;
        m_profileGroupStack.pop();
    }

void fe::profilerLogger::printToStream(std::ostream &out)
    {
        out << m_baseGroup->m_name << " {\n";
        for (auto &child : m_baseGroup->m_children) 
            {
                print(out, *child, "\t");
            }
        out << "}\n\n";
        m_baseGroup->m_children.clear();
    }

void fe::profilerLogger::setProfileGroup(fe::str group, bool profile)
    {
        m_allProfileGroups[group]->m_profile = profile;
    }

bool fe::profilerLogger::wantProfile(fe::str group)
    {
        return m_allProfileGroups.find(group) == m_allProfileGroups.end() || m_allProfileGroups[group]->m_profile;
    }

fe::profilerLogger::profileDynamicData &fe::profilerLogger::createProfile()
    {
        fe::singlyLinkedList<unsigned int>::node *currentNode = m_unallocatedProfiles.head();
        while (currentNode->m_next)
            {
                currentNode = currentNode->m_next;
            }

        unsigned int index = currentNode->m_data;
        m_unallocatedProfiles.remove(currentNode);

        m_allProfiles[index].init(index, this);
        return m_allProfiles[index];
    }

void fe::profilerLogger::destroyProfile(profileDynamicData &data)
    {
        m_unallocatedProfiles.insert(data.m_index);
    }

void fe::profilerLogger::clearTotalCalls()
    {
        for (unsigned int i = 0; i < m_profilesCreated; i++)
            {
                m_profileData[i].m_calls = 0;
            }
    }

void fe::profilerLogger::profileDynamicData::init(unsigned int index, profilerLogger *logger)
    {
        m_index = index;
        m_profileLogger = logger;
    }

void fe::profilerLogger::profileDynamicData::start(const char *name)
    {
        #if FE_PROFILE_RELEASE || _DEBUG
            m_profile = false;
            unsigned int strSize = std::strlen(name);
            if (m_nameSize < strSize)
                {
                    m_nameSize = strSize;
                    m_nameStr = static_cast<char*>(fe::memoryManager::get().alloc((m_nameSize + 1) * sizeof(char)));
                    std::memset(m_nameStr, 0, m_nameSize);
                }
            std::strcpy(m_nameStr, name);
            if (m_profileLogger->wantProfile(FE_STR(name)))
                {
                    m_profileLogger->startProfile(name);
			        m_startTime = fe::clock::getTimeSinceEpoch();
                    m_profile = true;
                }

        #endif
	}

void fe::profilerLogger::profileDynamicData::end()
    {
        #if FE_PROFILE_RELEASE || _DEBUG
            if (m_profile)
                {
			        m_endTime = fe::clock::getTimeSinceEpoch();
			        fe::time runtime = m_endTime - m_startTime;
                    m_profileLogger->endProfile(m_nameStr, runtime);
                    m_profileLogger->destroyProfile(*this);
                }
        #endif
	}

void fe::profilerLogger::profileDynamicData::shutDown()
    {
        //free(m_nameStr);
    }

fe::profilerLogger::profileGroup *fe::profilerLogger::profileGroup::init(const char *group)
    {
        unsigned int strSize = std::strlen(group);
        if (m_nameSize < strSize)
            {
                m_nameSize = strSize;
                m_name = static_cast<char*>(fe::memoryManager::get().alloc((m_nameSize + 1) * sizeof(char)));
                std::memset(m_name, 0, m_nameSize);
            }
        std::strcpy(m_name, group);
        return this;
    }

void fe::profilerLogger::profileGroup::shutDown()
    {
        //free(m_name);
    }
