// serializerID
// Allows the user to serialize data to a stream with IDs to
// get and set data easily
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../utilities.hpp"
#include "../../debug/logger.hpp"
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <utility>
#include <type_traits>
#include <stack>
#include <sstream>
#include <memory>

#include "../../typeDefines.hpp"
#ifdef FE_IS_ENGINE
    #include "../../debug/logger.hpp"
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
                public:
                    struct dataBlock 
                        {
                            std::string m_id;
                            std::unordered_map<std::string, std::string> m_mappedData;
                            std::unordered_map<std::string, std::vector<std::string>> m_mappedListPrimitiveData;
                            std::unordered_map<std::string, std::unique_ptr<dataBlock>> m_mappedObjectData;
                            std::unordered_map<std::string, std::vector<std::unique_ptr<dataBlock>>> m_mappedListObjectData;

                            int m_size; // sizeof the datablock. Defaults to zero as for this is only used for lists

                            dataBlock() : m_id(""), m_size(0) {}
                            dataBlock(const std::string &id) : m_id(id), m_size(0) {}

                            FLAT_ENGINE_API void outData(std::ostream &out, const char *preDataText = "\0");
                            FLAT_ENGINE_API void readData(const char *block);
                            FLAT_ENGINE_API bool hasData(const char *dataId);
                            FLAT_ENGINE_API ~dataBlock();

                            FLAT_ENGINE_API dataBlock &operator=(dataBlock &rhs);
                        };
                private:
                    const std::unique_ptr<dataBlock> m_baseDataBlock;
                    std::stack<dataBlock*> m_currentBlock; // current block we are writing to

                    FLAT_ENGINE_API void interpretData(const char *dataBlock);

                    template<typename T>
                    T toPrimitive(const std::string &data, T) { FE_LOG_WARNING("Data type unsupported to deserialize. fe::serializerID::toPrimitive"); return T(); }
                    FLAT_ENGINE_API unsigned long toPrimitive(const std::string &data, unsigned long);
                    FLAT_ENGINE_API int toPrimitive(const std::string &data, int);
                    FLAT_ENGINE_API unsigned int toPrimitive(const std::string &data, unsigned int);
                    FLAT_ENGINE_API float toPrimitive(const std::string &data, float);
                    FLAT_ENGINE_API bool toPrimitive(const std::string &data, bool);
                    FLAT_ENGINE_API std::string toPrimitive(const std::string &data, std::string);

                public:
                    FLAT_ENGINE_API serializerID();

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
        void serializerID::write(const std::string &id, T &&data)
            {
                if constexpr (fe::is_str<T>::value)
                    {
                        m_currentBlock.top()->m_mappedData[id] = data;
                    }
                else
                    {
                        m_currentBlock.top()->m_mappedData[id] = std::to_string(data);
                    }
            }

        template<typename T>
        void serializerID::write(const std::string &id, T &data)
            {
                if constexpr (fe::is_str<T>::value)
                    {
                        m_currentBlock.top()->m_mappedData[id] = data;
                    }
                else
                    {
                        write(id, std::forward<T>(data));
                    }
            }

        template<typename T>
        void serializerID::writeList(const std::string &id, T &&data)
            {
                if constexpr (fe::is_str<T>::value)
                    {
                        m_currentBlock.top()->m_mappedListPrimitiveData[id].emplace_back(data);
                    }
                else
                    {
                        m_currentBlock.top()->m_mappedListPrimitiveData[id].push_back(std::to_string(data));
                    }
            }

        template<typename T>
        void serializerID::writeList(const std::string &id, T &data)
            {
                writeList(id, std::forward<T>(data));
            }

        template<typename T>
        T serializerID::read(const std::string &id)
            {
                return toPrimitive(m_currentBlock.top()->m_mappedData[id], T());
            }

        template<typename T>
        T serializerID::readList(const std::string &id)
            {
                std::string lastData = m_currentBlock.top()->m_mappedListPrimitiveData[id].back();
                m_currentBlock.top()->m_mappedListPrimitiveData[id].erase(m_currentBlock.top()->m_mappedListPrimitiveData[id].end() - 1);
                return toPrimitive(lastData, T());
            }
    }

#define SERIALIZE_ID(...) [[deprecated]]void serialize(fe::serializerID&) { assert(false && "Deprecated"); } [[deprecated]]void deserialize(fe::serializerID&) { assert(false && "Deprecated"); }
#define SERIALIZE_CALLBACK_ID(...) [[deprecated]]void serialize(fe::serializerID&) { assert(false && "Deprecated"); } [[deprecated]]void deserialize(fe::serializerID&) { assert(false && "Deprecated"); }
#define SERIALIZE_NAME_ID(name, ...) [[deprecated]]void serialize##name(fe::serializerID&) { assert(false && "Deprecated"); } [[deprecated]]void deserialize##name(fe::serializerID&) { assert(false && "Deprecated"); }
