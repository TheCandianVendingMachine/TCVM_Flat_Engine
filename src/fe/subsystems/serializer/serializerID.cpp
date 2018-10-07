#include "fe/subsystems/serializer/serializerID.hpp"
#include "fe/subsystems/serializer/serializable.hpp"
#include <string.h>

#ifdef FE_IS_ENGINE
    #include "fe/feAssert.hpp"
#else
    #define FE_ASSERT(check, message) assert(check && message)
#endif

unsigned long fe::serializerID::toPrimitive(const std::string &data, unsigned long)
    {
        return std::stoul(data);
    }

int fe::serializerID::toPrimitive(const std::string &data, int)
    {
        return std::stoi(data);
    }

unsigned int fe::serializerID::toPrimitive(const std::string &data, unsigned int)
    {
        return std::stoul(data);
    }

float fe::serializerID::toPrimitive(const std::string &data, float)
    {
        return std::stof(data);
    }

bool fe::serializerID::toPrimitive(const std::string &data, bool)
    {
        return bool(std::stoi(data));
    }

std::string fe::serializerID::toPrimitive(const std::string &data, std::string)
    {
        return data;
    }

void fe::serializerID::writeObject(const std::string &id, const serializable &data)
    {
        m_writer->startObject(id);
        data.serialize(*this);
        m_writer->endObject();
    }

void fe::serializerID::writeObjectList(const std::string &id, const serializable &data)
    {
        m_writer->startObjectList(id);
        data.serialize(*this);
        m_writer->endObject();
    }

void fe::serializerID::readObject(const std::string &id, serializable &data)
    {
        m_writer->startObjectRead(id);
        data.deserialize(*this);
        m_writer->endObject();
    }

void fe::serializerID::readObjectList(const std::string &id, serializable &data)
    {
        m_writer->startObjectListRead(id);
        data.deserialize(*this);
        m_writer->endObject();
    }

bool fe::serializerID::listHasItems(const std::string &listID)
    {
        return m_writer->listHasItems(listID);
    }

void fe::serializerID::clearData()
    {
        m_writer->clearData();
    }

void fe::serializerID::outData(std::ostream &out)
    {
        m_writer->outData(out);
    }

void fe::serializerID::readData(std::istream &in)
    {
        m_writer->readData(in);
    }

fe::serializerID::~serializerID()
    {
        if (m_writer)
            {
                delete m_writer;
            }
    }
