// serializerID
// Allows the user to serialize data to a stream with IDs to
// get and set data easily
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/utilities.hpp"
#include <ostream>
#include <string>

#include "dataWriter.hpp"
#include "feDataWriter.hpp"

#include "fe/typeDefines.hpp"
#ifdef FE_IS_ENGINE
    #include "fe/debug/logger.hpp"
#else
    #define FE_LOG(...) ;
    #define FE_LOG_ERROR(...) ;
    #define FE_LOG_WARNING(...) ;
    #define FE_LOG_DEBUG(...) ;
    #define FE_CONSOLE_LOG(...) ;
#endif

namespace fe
    {
        class serializable;
        class serializerID
            {
                private:
                    fe::dataWriter *m_writer;

                    template<typename T>
                    T toPrimitive(const std::string &data, T) { FE_LOG_WARNING("Data type unsupported to deserialize. fe::serializerID::toPrimitive"); return T(); }
                    FLAT_ENGINE_API unsigned long toPrimitive(const std::string &data, unsigned long);
                    FLAT_ENGINE_API int toPrimitive(const std::string &data, int);
                    FLAT_ENGINE_API unsigned int toPrimitive(const std::string &data, unsigned int);
                    FLAT_ENGINE_API float toPrimitive(const std::string &data, float);
                    FLAT_ENGINE_API bool toPrimitive(const std::string &data, bool);
                    FLAT_ENGINE_API std::string toPrimitive(const std::string &data, std::string);
                    template<typename T>
                    std::string toString(T &data);

                public:
                    template<typename T = fe::feDataWriter>
                    serializerID();

                    // Write a single objet to serializer
                    FLAT_ENGINE_API void writeObject(const std::string &id, const serializable &data);
                    // Write an array objects to serializer
                    FLAT_ENGINE_API void writeObjectList(const std::string &id, const serializable &data);

                    // Read single objet to serializer
                    FLAT_ENGINE_API void readObject(const std::string &id, serializable &data);
                    // Read an array objects to serializer
                    FLAT_ENGINE_API void readObjectList(const std::string &id, serializable &data);

                    template<typename T>
                    void write(const std::string &id, T &&data);
                    template<typename T>
                    void write(const std::string &id, T &data);

                    template<typename T>
                    void writeList(const std::string &id, T &&data);
                    template<typename T>
                    void writeList(const std::string &id, T &data);

                    // Read data from the current serialized object
                    template<typename T>
                    T read(const std::string &id);
                    // Read data from an array from the current serialized object
                    template<typename T>
                    T readList(const std::string &id);

                    // Returns true as long as the listID has items
                    FLAT_ENGINE_API bool listHasItems(const std::string &listID);

                    FLAT_ENGINE_API void clearData();

                    FLAT_ENGINE_API void outData(std::ostream &out);
                    FLAT_ENGINE_API void readData(std::istream &in);

                    FLAT_ENGINE_API ~serializerID();
            };

        template<typename T>
        std::string serializerID::toString(T &data)
            {
                if constexpr (fe::is_str<T>::value)
                    {
                        return data;
                    }
                else
                    {
                        return std::to_string(data);
                    }
            }

        template<typename T>
        serializerID::serializerID()
            {
                static_assert(std::is_base_of<fe::dataWriter, T>::value, "T is not child of fe::dataWriter");
                m_writer = new T();
            }

        template<typename T>
        void serializerID::write(const std::string &id, T &&data)
            {
                m_writer->write(id, toString(data));
            }

        template<typename T>
        void serializerID::write(const std::string &id, T &data)
            {
                write(id, std::forward<T>(data));
            }

        template<typename T>
        void serializerID::writeList(const std::string &id, T &&data)
            {
                m_writer->writeList(id, toString(data));
            }

        template<typename T>
        void serializerID::writeList(const std::string &id, T &data)
            {
                writeList(id, std::forward<T>(data));
            }

        template<typename T>
        T serializerID::read(const std::string &id)
            {
                return toPrimitive(m_writer->read(id), T());
            }

        template<typename T>
        T serializerID::readList(const std::string &id)
            {
                return toPrimitive(m_writer->readList(id), T());
            }
    }

#define SERIALIZE_ID(...) [[deprecated]]void serialize(fe::serializerID&) { assert(false && "Deprecated"); } [[deprecated]]void deserialize(fe::serializerID&) { assert(false && "Deprecated"); }
#define SERIALIZE_CALLBACK_ID(...) [[deprecated]]void serialize(fe::serializerID&) { assert(false && "Deprecated"); } [[deprecated]]void deserialize(fe::serializerID&) { assert(false && "Deprecated"); }
#define SERIALIZE_NAME_ID(name, ...) [[deprecated]]void serialize##name(fe::serializerID&) { assert(false && "Deprecated"); } [[deprecated]]void deserialize##name(fe::serializerID&) { assert(false && "Deprecated"); }

