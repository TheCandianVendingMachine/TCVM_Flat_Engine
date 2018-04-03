// guid.hpp
// a Global Unique IDentifier
#pragma once
#include "../typeDefines.hpp"
#include "../time/clock.hpp"
#include "../math/random.hpp"

namespace fe
    {
        class guid
            {
                private:
                    fe::int64 m_guid = -1;

                public:
                    guid()
                        {
                            // should always generate a good GUID due to the probability of generating two objects within microseconds
                            // and due to the chance of getting two random values that are exactly the same overlapping
                            m_guid = fe::clock::getTimeSinceEpoch().asMicroseconds() + fe::random::get().generate(0, std::numeric_limits<int>::max());
                        }

                    fe::int64 getGUID() const { return m_guid; }
                    virtual bool operator==(const fe::guid &rhs)
                        {
                            return m_guid == rhs.m_guid;
                        }
            };
    }