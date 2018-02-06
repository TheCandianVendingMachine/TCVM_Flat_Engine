// serialzable.hpp
// a base class interface for other classes to extend to allow the ability to serialize it
#pragma once

namespace fe 
    {
        class serializerID;
        class serializable
            {
                public:
                    virtual void serialize(fe::serializerID &serializer) const { }
                    virtual void deserialize(fe::serializerID &serializer) { }
            };
    }