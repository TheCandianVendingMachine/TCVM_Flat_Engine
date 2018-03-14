// profilerData.hpp
// The data which the profiler gained.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../time/time.hpp"
#include "../typeDefines.hpp"
#include <unordered_map>
#include <ostream>
#include <stack>

namespace fe
    {
        class profilerLogger
            {
                private:
                    struct profileData
                        {
                            fe::time m_time[FE_PROFILER_AVERAGE_MAX];
                            fe::time m_avgTime;
                            // How many times this profile instance was called in a frame
                            unsigned int m_calls = 0;
                            // Total amount of calls - Does not get reset
                            unsigned int m_totalCalls = 0;
                        } m_profileData[FE_MAX_PROFILER_PROFILES];
                    unsigned int m_profilesCreated;

                    struct profileGroup
                        {
                            profileData *m_data;
                            std::vector<profileGroup*> m_children;

                            char m_name[512] = "\0";

                            bool m_profile = true;

                            profileGroup *init(const char *group)
                                {
                                    std::strcpy(m_name, group);
                                    return this;
                                }

                            bool operator<(const profileGroup &rhs) const { return m_data->m_avgTime < rhs.m_data->m_avgTime; }
                            bool operator>(const profileGroup &rhs) const { return m_data->m_avgTime > rhs.m_data->m_avgTime; }
                        } m_profileGroup[FE_MAX_PROFILER_PROFILES];
                        unsigned int m_groupsCreated;

                    std::unordered_map<fe::str, profileGroup*> m_allProfileGroups;
                    std::stack<profileGroup*> m_profileGroupStack;
                    profileGroup *m_baseGroup;

                    static profilerLogger *m_instance;

                    FLAT_ENGINE_API __forceinline void print(std::ostream &out, profileData &data, const char *prefix);
                    FLAT_ENGINE_API void print(std::ostream &out, profileGroup &group, const char *prefix);

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API static fe::profilerLogger &get();

                    FLAT_ENGINE_API void startProfile(const char *profile);
                    FLAT_ENGINE_API void endProfile(const char *profile, fe::time time);
                    FLAT_ENGINE_API void printToStream(std::ostream &out);
                    FLAT_ENGINE_API void printToStream(fe::str group, std::ostream &out);

                    FLAT_ENGINE_API void setProfileGroup(fe::str group, bool profile);
                    FLAT_ENGINE_API bool wantProfile(fe::str group);

                    // Clears all profiles of the amount of calls in the frame
                    FLAT_ENGINE_API void clearTotalCalls();

            };
    }