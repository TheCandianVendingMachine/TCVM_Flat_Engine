// dataWriter.hpp
// base class for all data writers for serializer
#pragma once
#include <ostream>
#include <string>

namespace fe
    {
        class serializable;
        class dataWriter
            {
                public:
                    virtual void setTitle(const std::string &title) = 0;

                    virtual void startObject(const std::string &id) = 0;
                    virtual void startObjectList(const std::string &id) = 0;
                    virtual void startObjectRead(const std::string &id) = 0;
                    virtual void startObjectListRead(const std::string &id) = 0;
                    virtual void endObjectList() = 0;
                    virtual void endObject() = 0;

                    virtual void write(const std::string &id, const std::string &value) = 0;
                    virtual void writeList(const std::string &id, const std::string &value) = 0;

                    virtual std::string read(const std::string &id) = 0;
                    virtual std::string readList(const std::string &id) = 0;

                    virtual void outData(std::ostream &out) = 0;
                    virtual void readData(std::istream &in) = 0;

                    virtual bool listHasItems(const std::string &list) = 0;

                    virtual void clearData() = 0;

            };
    }
