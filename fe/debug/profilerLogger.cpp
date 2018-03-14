#include "profilerLogger.hpp"
#include "../objectManagement/str.hpp"
#include <algorithm>

fe::profilerLogger *fe::profilerLogger::m_instance = nullptr;

void fe::profilerLogger::print(std::ostream &out, unsigned int profileIndex, const char *prefix)
    {
        profileData data = m_profileData[profileIndex];
        fe::time avg;
        for (unsigned int j = 0; j < std::min(data.m_calls, 500u); j++)
            {
                avg += data.m_time[j];
            }

        if (data.m_calls > 0)
            {
                avg /= std::min(data.m_calls, 500u);
            }

    #if !FE_PROFILE_PRINT_ZEROS
        if (avg.asMicroseconds() > 0)
    #endif
        {
            out <<          prefix << data.m_name           << " {"
                   "\n" <<  prefix << "\tCalls per frame: " << data.m_calls <<
                   "\n" <<  prefix << "\tMicroseconds: "    << avg.asMicroseconds() <<
                   "\n" <<  prefix << "\tMilliseconds: "    << avg.asMilliseconds() <<
                   "\n" <<  prefix << "\tSeconds: "         << avg.asSeconds() <<
                   "\n" <<  prefix << "\tFPS: "             << 1 / avg.asSeconds() << "\n" << prefix << "}\n";
        }
    }

void fe::profilerLogger::print(std::ostream &out, profileGroup &group, const char *prefix)
    {
        out << prefix << group.m_groupName << " {\n";
        char newPrefix[16];
        std::strcpy(newPrefix, prefix);
        std::strcat(newPrefix, "\t");
        for (auto &profile : group.m_profiles)
            {
                print(out, profile.second, newPrefix);
            }

        for (unsigned int i = 0; i < group.m_groupChildrenIndex; i++)
            {
                print(out, m_profileGroups[group.m_groupChildren[i]], newPrefix);
            }
        group.m_groupChildrenIndex = 0;
        out << prefix << "}\n\n";
    }

void fe::profilerLogger::startUp()
    {
        if (!m_instance) 
            {
                m_instance = this;
                std::strcpy(m_baseGroup.m_groupName, "profile_results");
                m_currentGroupStackIndex = 0;
                m_currentGroupStack[m_currentGroupStackIndex] = &m_baseGroup;
            }
    }

void fe::profilerLogger::shutDown()
    {
        m_instance = nullptr;
    }

fe::profilerLogger &fe::profilerLogger::get()
    {
        return *m_instance;
    }

void fe::profilerLogger::startProfile(const char *group)
    {
        profileGroup &groupObj = m_profileGroups[FE_STR(group)];
        if (groupObj.m_new)
            {
                std::strcpy(groupObj.m_groupName, group);
                groupObj.m_new = false;
            }

        profileGroup *currentGroup = m_currentGroupStack[m_currentGroupStackIndex++];
        if (std::strcmp(group, currentGroup->m_groupName) != 0) 
            {
                bool hasValue = false;
                for (unsigned int i = 0; i < currentGroup->m_groupChildrenIndex; i++)
                    {
                        if (currentGroup->m_groupChildren[i] == FE_STR(group))
                            {
                                hasValue = true;
                                break;
                            }
                    }

                if (!hasValue) 
                    {
                        currentGroup->m_groupChildren[currentGroup->m_groupChildrenIndex++] = FE_STR(group);
                    }
            }
        m_currentGroupStack[m_currentGroupStackIndex] = &groupObj;
    }

void fe::profilerLogger::endProfile(const char *group, const char *profile, fe::time time)
    {
        profileGroup &groupObj = m_profileGroups[FE_STR(group)];
        if (groupObj.m_profiles.find(FE_STR(profile)) == groupObj.m_profiles.end())
            {
                profileData &data = m_profileData[m_profilesCreated];
                std::strcpy(data.m_name, profile);
                groupObj.m_profiles[FE_STR(profile)] = m_profilesCreated++;
            }

        unsigned int profileIndex = groupObj.m_profiles[FE_STR(profile)];
        profileData &data = m_profileData[profileIndex];
        if (data.m_calls < FE_PROFILER_AVERAGE_MAX)
            {
                data.m_time[data.m_calls] = time;
            }
        data.m_calls++;
        data.m_totalCalls++;
        m_currentGroupStackIndex--;
    }

void fe::profilerLogger::printToStream(std::ostream &out)
    {
        print(out, m_baseGroup);
    }

void fe::profilerLogger::printToStream(fe::str group, std::ostream &out)
    {
        print(out, m_profileGroups[group]);
    }

void fe::profilerLogger::setProfileGroup(fe::str group, bool profile)
    {
        m_profileGroups[group].m_profile = profile;
    }

bool fe::profilerLogger::wantProfile(fe::str group)
    {
        return m_profileGroups.find(group) == m_profileGroups.end() || m_profileGroups[group].m_profile;
    }

void fe::profilerLogger::clearTotalCalls()
    {
        for (unsigned int i = 0; i < m_profilesCreated; i++)
            {
                m_profileData[i].m_calls = 0;
            }
    }
