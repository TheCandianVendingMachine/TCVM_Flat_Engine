#include "gameEvent.hpp"

bool fe::gameEvent::operator<=(const gameEvent &rhs) const
    {
        if (rhs.sendTimeType == timeVar::FRAME)
            {
                return (rhs.sendTimeVar.FRAME < sendTimeVar.FRAME);
            }
        return false;
    }

bool fe::gameEvent::operator<(const gameEvent &rhs) const
    {
        if (rhs.sendTimeType == timeVar::TIME)
            {
                return (rhs.sendTimeVar.TIME < sendTimeVar.TIME);
            }
        return false;
    }