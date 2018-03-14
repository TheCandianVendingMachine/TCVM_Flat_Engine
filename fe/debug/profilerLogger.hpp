// profilerData.hpp
// The data which the profiler gained.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../time/time.hpp"
#include "../typeDefines.hpp"
#include <unordered_map>
#include <ostream>

namespace fe
    {
        class profilerLogger
            {
                private:
                    struct profileData
                        {
                            fe::time m_time[FE_PROFILER_AVERAGE_MAX];
                            char m_name[512] = "\0";
                            // How many times this profile instance was called in a frame
                            unsigned int m_calls = 0;
                            // Total amount of calls - Does not get reset
                            unsigned int m_totalCalls = 0;
                        };

                    struct profileGroup
                        {
                            std::unordered_map<fe::str, unsigned int> m_profiles;
                            char m_groupName[512] = "\0";
                            fe::str m_groupChildren[25]; // group ID's that are children (new groups underneath this one) to this group
                            unsigned int m_groupChildrenIndex = 0;
                            bool m_profile = true;
                            bool m_new = true;
                        };

                    std::unordered_map<fe::str, profileGroup> m_profileGroups;
                    fe::profilerLogger::profileData m_profileData[FE_MAX_PROFILER_PROFILES];

                    profileGroup m_baseGroup;
                    profileGroup *m_currentGroupStack[25];

                    unsigned int m_currentGroupStackIndex;
                    unsigned int m_profilesCreated;

                    static profilerLogger *m_instance;

                    FLAT_ENGINE_API __forceinline void print(std::ostream &out, unsigned int profileIndex, const char *prefix = "");
                    FLAT_ENGINE_API void print(std::ostream &out, profileGroup &group, const char *prefix = "");

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API static fe::profilerLogger &get();

                    FLAT_ENGINE_API void startProfile(const char *group);
                    FLAT_ENGINE_API void endProfile(const char *group, const char *profile, fe::time time);
                    FLAT_ENGINE_API void printToStream(std::ostream &out);
                    FLAT_ENGINE_API void printToStream(fe::str group, std::ostream &out);

                    FLAT_ENGINE_API void setProfileGroup(fe::str group, bool profile);
                    FLAT_ENGINE_API bool wantProfile(fe::str group);

                    // Clears all profiles of the amount of calls in the frame
                    FLAT_ENGINE_API void clearTotalCalls();

            };
    }