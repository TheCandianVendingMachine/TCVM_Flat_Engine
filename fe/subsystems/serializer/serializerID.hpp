// serializerID
// Allows the user to serialize data to a stream with IDs to
// get and set data easily
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <utility>
#include <type_traits>
#include <sstream>
#include <memory>

namespace fe
    {
        class serializerID
            {
                public:
                    struct dataBlock 
                        {
                            std::string m_id;
                            std::unordered_map<std::string, std::string> m_mappedData;
                            std::vector<std::pair<std::string, std::unique_ptr<dataBlock>>> m_childDataBlocks;

                            bool m_read; // if this block has already been read, we dont want
                                         // to get data from it again
                            bool m_child;

                            dataBlock() : m_id(""), m_read(false), m_child(false) {}

                            FLAT_ENGINE_API void outData(std::ostream &out, const char *preDataText = "\0");
                            FLAT_ENGINE_API void readData(const char *block);
                            FLAT_ENGINE_API bool hasData(const char *dataId);
                        };
                private:

                    std::vector<std::unique_ptr<dataBlock>> m_data;

                    template<typename T>
                    T convertValue(const std::string &in, T val) { return T(); }
                    template<>
                    int convertValue(const std::string &in, int val);
                    template<>
                    unsigned int convertValue(const std::string &in, unsigned int val);
                    template<>
                    unsigned long convertValue(const std::string &in, unsigned long val);
                    template<>
                    float convertValue(const std::string &in, float val);
                    template<>
                    double convertValue(const std::string &in, double val);
                    template<>
                    bool convertValue(const std::string &in, bool val);

                    FLAT_ENGINE_API dataBlock *getDataBlock(dataBlock *initial, const char *id);
                    FLAT_ENGINE_API void interpretData(const char *dataBlock);

                public:
                    void serialize() {}
                    void serialize(dataBlock&) {}
                    void deserialize(dataBlock&) {}
                    void deserializeData(dataBlock&, const char*) {}

                    template<typename T, typename std::enable_if<!std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeData(dataBlock &block, const char *id, T &&data);

                    template<typename T, typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeData(dataBlock &block, const char *id, T &&data);

                    template<typename T, typename ...Args>
                    void serialize(dataBlock &block, const char *id, T &&data, Args &&...args);

                    template<typename ...Args>
                    void serializeBlock(dataBlock &block, const char *blockID, Args &&...Args);

                    template<typename ...Args>
                    void serializeBlock(const char *blockID, Args &&...args);

                    template<typename T>
                    void deserializeData(dataBlock &dataBlock, const char *id, T &newValue);

                    template<typename T, typename ...Args>
                    void deserialize(dataBlock &dataBlock, const char *id, T &newValue, Args &&...args);

                    template<typename T, typename ...Args>
                    bool deserializeBlock(const char *blockID, const char *id, T &newValue, Args &&...args);

                    FLAT_ENGINE_API void clearData();

                    FLAT_ENGINE_API void outData(std::ostream &out)
                        {
                            for (auto &data : m_data)
                                {
                                    data->outData(out);
                                }
                        }

                    FLAT_ENGINE_API void readData(std::istream &in);

                    // Returns true as long as the requested serialized data exists
                    FLAT_ENGINE_API bool objectExists(const char *id);
            };

        template<>
        inline int serializerID::convertValue(const std::string &in, int val)
            {
                if (in.empty()) return int();
                return std::stoi(in);
            }

        template<>
        inline unsigned int serializerID::convertValue(const std::string &in, unsigned int val)
            {
                if (in.empty()) return unsigned int();
                return std::stoul(in);
            }

        template<>
        inline unsigned long serializerID::convertValue(const std::string &in, unsigned long val)
            {
                if (in.empty()) return unsigned int();
                return std::stoul(in);
            }

        template<>
        inline float serializerID::convertValue(const std::string &in, float val)
            {
                if (in.empty()) return float();
                return std::stof(in);
            }

        template<>
        inline double serializerID::convertValue(const std::string &in, double val)
            {
                if (in.empty()) return double();
                return std::stod(in);
            }

        template<>
        inline bool serializerID::convertValue(const std::string &in, bool val)
            {
                if (in.empty()) return bool();
                std::string boolStr = in;
                std::transform(boolStr.begin(), boolStr.end(), boolStr.begin(), std::tolower);

                if (boolStr == "true")
                    { 
                        return true;
                    }
                else if (boolStr == "false")
                    {
                        return false;
                    }

                return std::stoi(in);
            }

        template<typename T, typename std::enable_if<!std::is_class<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::serializeData(dataBlock &block, const char *id, T &&data)
            {
                block.m_mappedData[id] = std::to_string(data);
            }

        template<typename T, typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::serializeData(dataBlock &block, const char *id, T &&data)
            {
                block.m_childDataBlocks[id].reset(new dataBlock);
                data.serialize(*this, *block.m_childDataBlocks[id].get());
            }

        template<typename T, typename ...Args>
        void serializerID::serialize(dataBlock &block, const char *id, T &&data, Args &&...args)
            {
                serializeData(block, id, data);
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename ...Args>
        void fe::serializerID::serializeBlock(dataBlock &block, const char *blockID, Args &&...args)
            {
                block.m_id = blockID;
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename ...Args>
        void serializerID::serializeBlock(const char *blockID, Args &&...args)
            {
                m_data.emplace_back(new dataBlock);
                dataBlock *block = m_data.back().get();
                block->m_id = blockID;
                serialize(*block, std::forward<Args>(args)...);
            }

        template<typename T>
        void serializerID::deserializeData(dataBlock &dataBlock, const char *id, T &newValue)
            {
                newValue = convertValue(dataBlock.m_mappedData[id], newValue);
            }

        template<typename T, typename ...Args>
        void serializerID::deserialize(dataBlock &dataBlock, const char *id, T &newValue, Args &&...args)
            {
                deserializeData(dataBlock, id, newValue);
                deserialize(dataBlock, std::forward<Args>(args)...);
            }

        template<typename T, typename ...Args>
        bool serializerID::deserializeBlock(const char *blockID, const char *id, T &newValue, Args &&...args)
            {
                dataBlock *selectedBlock = nullptr;
                for (auto &datBlock : m_data)
                    {
                        selectedBlock = getDataBlock(datBlock.get(), blockID);
                        if (selectedBlock) break;
                    }

                if (selectedBlock) 
                    {
                        selectedBlock->m_read = true;
                        deserialize(*selectedBlock, id, newValue, args...);
                        return true;
                    }
                return false;
            }
    }

#define SERIALIZE_ID(...) \
void serialize(fe::serializerID &serial) const { serial.serializeBlock(__VA_ARGS__); }\
bool deserialize(fe::serializerID &serial) { return serial.deserializeBlock(__VA_ARGS__); }\
void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); }