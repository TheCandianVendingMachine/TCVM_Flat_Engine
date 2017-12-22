#include "profilerLogger.hpp"
#include "../objectManagement/str.hpp"
#include <algorithm>

fe::profilerLogger *fe::profilerLogger::m_instance = nullptr;

void fe::profilerLogger::startUp()
    {
        if (!m_instance) 
            {
                m_instance = this;
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

void fe::profilerLogger::add(const char *profile, fe::time time)
    {
        if (m_profiles.find(FE_STR(profile)) == m_profiles.end())
            {
                m_profiles[FE_STR(profile)] = m_profilesCreated;
                std::strcpy(m_profileData[m_profilesCreated++].m_name, profile);
            }

        profileData &data = m_profileData[m_profiles[FE_STR(profile)]];
        if (data.m_calls < FE_PROFILER_AVERAGE_MAX) 
            {
                data.m_time[data.m_calls] = time;
            }
        data.m_calls += 1;
        data.m_totalCalls++;
    }

void fe::profilerLogger::add(const char *group, const char *profile, fe::time time)
    {
        bool groupMade = m_profileGroups.find(FE_STR(group)) != m_profileGroups.end();
        bool profileAdded = m_profiles.find(FE_STR(profile)) != m_profiles.end();
        m_nonProfileGroups[FE_STR(group)] = true;
        add(profile, time);

        if (!groupMade)
            {
                auto groupPair = &m_profileGroups[FE_STR(group)];
                groupPair->first[groupPair->second++] = FE_STR(profile);
            }

        if (!profileAdded)
            {
                std::strcpy(m_profileData[m_profiles[FE_STR(profile)]].m_group, group);
            }
    }

void fe::profilerLogger::printToStream(std::ostream &out)
    {
        for (unsigned int i = 0; i < m_profilesCreated; i++)
            {
                profileData data = m_profileData[i];
                fe::time avg;
                for (unsigned int j = 0; j < std::min(data.m_calls, 500u); j++)
                    {
                        avg += data.m_time[j];
                    }

                if (data.m_calls > 0)
                    {
                        avg /= std::min(data.m_calls, 500u);
                    }

            #if FE_PROFILE_PRINT_ZEROS
                out <<  data.m_group << "::" << data.m_name <<
                        "\nCalls per frame: "   << data.m_calls <<
                        "\nMicroseconds: "      << avg.asMicroseconds() <<
                        "\nMilliseconds: "      << avg.asMilliseconds() <<
                        "\nSeconds: "           << avg.asSeconds() << "\n\n";
            #else
                if (avg.asMicroseconds() > 0)
                    {
                        out <<  data.m_group << "::" << data.m_name <<
                                "\nCalls per frame: "   << data.m_calls <<
                                "\nMicroseconds: "      << avg.asMicroseconds() <<
                                "\nMilliseconds: "      << avg.asMilliseconds() <<
                                "\nSeconds: "           << avg.asSeconds() << "\n\n";
                    }
            #endif
            }
    }

void fe::profilerLogger::printToStream(fe::str group, std::ostream &out)
    {
        auto profiles = m_profileGroups[group];
        for (unsigned int i = 0; i < profiles.second; i++)
            {
                profileData data = m_profileData[i];
                fe::time avg;
                for (unsigned int j = 0; j < std::min(data.m_calls, 500u); j++)
                    {
                        avg += data.m_time[j];
                    }

                if (data.m_calls > 0)
                    {
                        avg /= std::min(data.m_calls, 500u);
                    }

            #if FE_PROFILE_PRINT_ZEROS
                out <<  data.m_group << "::" << data.m_name <<
                        "\nCalls per frame: "   << data.m_calls <<
                        "\nMicroseconds: "      << avg.asMicroseconds() <<
                        "\nMilliseconds: "      << avg.asMilliseconds() <<
                        "\nSeconds: "           << avg.asSeconds() << "\n\n";
            #else
                if (avg.asMicroseconds() > 0)
                    {
                        out <<  data.m_group << "::" << data.m_name <<
                                "\nCalls per frame: "   << data.m_calls <<
                                "\nMicroseconds: "      << avg.asMicroseconds() <<
                                "\nMilliseconds: "      << avg.asMilliseconds() <<
                                "\nSeconds: "           << avg.asSeconds() << "\n\n";
                    }
            #endif
            }
    }

void fe::profilerLogger::profileGroup(fe::str group, bool profile)
    {
        m_nonProfileGroups[group] = profile;
    }

bool fe::profilerLogger::wantProfile(fe::str group)
    {
        return !(m_nonProfileGroups.find(group) != m_nonProfileGroups.end()) || (m_nonProfileGroups.find(group) != m_nonProfileGroups.end() && m_nonProfileGroups[group]);
    }

void fe::profilerLogger::clearTotalCalls()
    {
        for (unsigned int i = 0; i < m_profilesCreated; i++)
            {
                m_profileData[i].m_calls = 0;
            }
    }
