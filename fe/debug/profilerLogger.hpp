// profilerData.hpp
// The data which the profiler gained.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../time/time.hpp"
#include "../typeDefines.hpp"
#include <map>
#include <ostream>

namespace fe
    {
        class profilerLogger
            {
                private:
                    struct profileData
                        {
                            fe::time m_time[FE_PROFILER_AVERAGE_MAX];
                            char m_name[512];
                            char m_group[512];
                            unsigned int m_calls;
                            unsigned int m_totalCalls;
                        };

                    // All groups and the profiles that are associated with the groups. The pair contains the array of profiles and the last index of the profiles
                    std::map<fe::guid, std::pair<fe::guid[FE_MAX_PROFILER_PROFILES], unsigned int>> m_profileGroups;
                    std::map<fe::guid, unsigned int> m_profiles;
                    fe::profilerLogger::profileData m_profileData[FE_MAX_PROFILER_PROFILES];
                    std::map<fe::guid, bool> m_nonProfileGroups;

                    unsigned int m_profilesCreated;

                    static profilerLogger *m_instance;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API static fe::profilerLogger &get();

                    FLAT_ENGINE_API void add(const char *profile, fe::time time);
                    FLAT_ENGINE_API void add(const char *group, const char *profile, fe::time time);
                    FLAT_ENGINE_API void printToStream(std::ostream &out);
                    FLAT_ENGINE_API void printToStream(fe::guid group, std::ostream &out);

                    FLAT_ENGINE_API void profileGrouo(fe::guid group, bool profile);
                    FLAT_ENGINE_API bool wantProfile(fe::guid group);

                    // Clears all profiles of the amount of calls in the frame
                    FLAT_ENGINE_API void clearTotalCalls();

            };
    }