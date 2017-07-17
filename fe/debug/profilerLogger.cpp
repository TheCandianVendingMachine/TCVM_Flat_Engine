#include "profilerLogger.hpp"
#include "../objectManagement/guid.hpp"

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
                data.m_time[data.m_calls++] = time;
            }
        data.m_totalCalls++;
    }

void fe::profilerLogger::add(const char *group, const char *profile, fe::time time)
    {
        bool groupMade = m_profileGroups.find(FE_STR(group)) != m_profileGroups.end();
        add(profile, time);

        if (!groupMade)
            {
                auto groupPair = &m_profileGroups[FE_STR(group)];
                groupPair->first[groupPair->second++] = FE_STR(profile);

                std::strcpy(m_profileData[m_profiles[FE_STR(profile)]].m_group, group);
            }
    }

void fe::profilerLogger::printToStream(std::ostream &out)
    {
        for (unsigned int i = 0; i < m_profilesCreated; i++)
            {
                profileData data = m_profileData[i];
                fe::time avg;
                for (int j = 0; j < data.m_calls; j++)
                    {
                        avg += data.m_time[j];
                    }

                if (data.m_calls > 0)
                    {
                        avg /= data.m_calls;
                    }

            #if FE_PROFILE_PRINT_ZEROS
                out <<  data.m_group << "::" << data.m_name <<
                        "\nCalls per frame: "   << data.m_totalCalls <<
                        "\nMicroseconds: "      << avg.asMicroseconds() <<
                        "\nMilliseconds: "      << avg.asMilliseconds() <<
                        "\nSeconds: "           << avg.asSeconds() << "\n\n";
            #else
                if (avg.asMicroseconds() > 0)
                    {
                        out <<  data.m_group << "::" << data.m_name <<
                                "\nCalls per frame: "   << data.m_totalCalls <<
                                "\nMicroseconds: "      << avg.asMicroseconds() <<
                                "\nMilliseconds: "      << avg.asMilliseconds() <<
                                "\nSeconds: "           << avg.asSeconds() << "\n\n";
                    }
            #endif
            }
    }

void fe::profilerLogger::printToStream(fe::guid group, std::ostream &out)
    {
        auto profiles = m_profileGroups[group];
        for (int i = 0; i < profiles.second; i++)
            {
                profileData data = m_profileData[m_profiles[profiles.first[i]]];
                fe::time avg;
                for (int j = 0; j < data.m_calls - 1; j++)
                    {
                        avg += data.m_time[j];
                    }
                avg /= data.m_calls - 1;

            #if FE_PROFILE_PRINT_ZEROS
                out <<  data.m_group << "::" << data.m_name <<
                        "\nCalls per frame: "   << data.m_totalCalls <<
                        "\nMicroseconds: "      << avg.asMicroseconds() <<
                        "\nMilliseconds: "      << avg.asMilliseconds() <<
                        "\nSeconds: "           << avg.asSeconds() << "\n\n";
            #else
                if (avg.asMicroseconds() > 0)
                    {
                        out <<  data.m_group << "::" << data.m_name <<
                                "\nCalls per frame: "   << data.m_totalCalls <<
                                "\nMicroseconds: "      << avg.asMicroseconds() <<
                                "\nMilliseconds: "      << avg.asMilliseconds() <<
                                "\nSeconds: "           << avg.asSeconds() << "\n\n";
                    }
            #endif
            }
    }

void fe::profilerLogger::clearTotalCalls()
    {
        for (unsigned int i = 0; i < m_profilesCreated; i++)
            {
                m_profileData[i].m_calls = 0;
                m_profileData[i].m_totalCalls = 0;
            }
    }
