// serializerID
// Allows the user to serialize data to a stream with IDs to
// get and set data easily
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../utilities.hpp"
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
                            std::unordered_map<std::string, std::vector<std::string>> m_mappedListPrimitiveData;
                            std::unordered_map<std::string, std::vector<std::unique_ptr<dataBlock>>> m_mappedListObjectData;
                            std::vector<std::pair<std::string, std::unique_ptr<dataBlock>>> m_childDataBlocks;

                            bool m_read; // if this block has already been read, we dont want to get data from it again
                            bool m_child;

                            dataBlock() : m_id(""), m_read(false), m_child(false) {}

                            FLAT_ENGINE_API void outData(std::ostream &out, const char *preDataText = "\0");
                            FLAT_ENGINE_API void readData(const char *block);
                            FLAT_ENGINE_API bool hasData(const char *dataId);
                            FLAT_ENGINE_API ~dataBlock();
                        };
                private:

                    std::vector<std::unique_ptr<dataBlock>> m_data;

                    template<typename T>
                    T convertValue(const std::string &in, T val = T()) { return T(); }
                    int convertValue(const std::string &in, int val);
                    unsigned int convertValue(const std::string &in, unsigned int val);
                    unsigned long convertValue(const std::string &in, unsigned long val);
                    float convertValue(const std::string &in, float val);
                    double convertValue(const std::string &in, double val);
                    bool convertValue(const std::string &in, bool val);
                    std::string convertValue(const std::string &in, const char *val);
                    std::string convertValue(const std::string &in, char *val);

                    void serialize() {}
                    void serialize(dataBlock&) {}
                    void deserialize(dataBlock&) {}
                    void deserializeData(dataBlock&, const char*) {}
                    
                    template<typename T, typename std::enable_if<!std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeListData(dataBlock &block, const char *id, T &&data);

                    template<typename T, typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeListData(dataBlock &block, const char *id, T &&data);


                    template<typename T, typename std::enable_if<!std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeData(dataBlock &block, const char *id, T &&data);

                    void serializeData(dataBlock &block, const char *id, const char *data);
                    void serializeData(dataBlock &block, const char *id, char *data);

                    template<typename T, typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeData(dataBlock &block, const char *id, T &&data);


                    template<typename T, typename ...Args, typename std::enable_if<!fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serialize(dataBlock &block, const char *id, T &&dataVec, Args &&...args);

                    template<typename T, typename ...Args, typename std::enable_if<fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serialize(dataBlock &block, const char *id, T &&data, Args &&...args);

                    template<typename T, typename std::enable_if<fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void deserializeData(dataBlock &dataBlock, const char *id, T &newValue);

                    template<typename T, typename std::enable_if<!fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void deserializeData(dataBlock &dataBlock, const char *id, T &newValue);

                    void deserializeData(dataBlock &dataBlock, const char *id, char *newValue);

                    template<typename T, typename ...Args>
                    void deserialize(dataBlock &dataBlock, const char *id, T &newValue, Args &&...args);


                    FLAT_ENGINE_API dataBlock *getDataBlock(dataBlock *initial, const char *id);
                    FLAT_ENGINE_API dataBlock *getDataBlock(dataBlock *initial);
                    FLAT_ENGINE_API void interpretData(const char *dataBlock);

                public:
                    template<typename ...Args>
                    void serializeBlock(const char *blockID, Args &&...args);
                    template<typename ...Args>
                    void serializeBlock(dataBlock &block, const char *blockID, Args &&...Args);
                    template<typename T, typename ...Args>
                    bool deserializeBlock(const char *blockID, const char *id, T &newValue, Args &&...args);
                    template<typename T, typename ...Args>
                    bool deserializeBlock(std::vector<std::unique_ptr<dataBlock>> &blocks, const char*, const char *id, T &newValue, Args &&...args);

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

                    FLAT_ENGINE_API ~serializerID();
            };

        inline int serializerID::convertValue(const std::string &in, int val)
            {
                if (in.empty()) return int();
                return std::stoi(in);
            }

        inline unsigned int serializerID::convertValue(const std::string &in, unsigned int val)
            {
                if (in.empty()) return unsigned int();
                return std::stoul(in);
            }

        inline unsigned long serializerID::convertValue(const std::string &in, unsigned long val)
            {
                if (in.empty()) return unsigned int();
                return std::stoul(in);
            }

        inline float serializerID::convertValue(const std::string &in, float val)
            {
                if (in.empty()) return float();
                return std::stof(in);
            }

        inline double serializerID::convertValue(const std::string &in, double val)
            {
                if (in.empty()) return double();
                return std::stod(in);
            }

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

        inline std::string serializerID::convertValue(const std::string &in, const char *val)
            {
                if (in.empty()) return "";
                return val;
            }

        inline std::string serializerID::convertValue(const std::string &in, char *val)
            {
                if (in.empty()) return "";
                return val;
            }

        template<typename T, typename std::enable_if<!std::is_class<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::serializeListData(dataBlock &block, const char *id, T &&data)
            {
                block.m_mappedListPrimitiveData[id].push_back(std::to_string(data));
            }

        template<typename T, typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::serializeListData(dataBlock &block, const char *id, T &&data)
            {
                block.m_mappedListObjectData[id].emplace_back(new dataBlock);
                block.m_mappedListObjectData[id].back()->m_id = id;
                data.serialize(*this, *block.m_mappedListObjectData[id].back().get());
            }

        template<typename T, typename std::enable_if<!std::is_class<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::serializeData(dataBlock &block, const char *id, T &&data)
            {
                block.m_mappedData[id] = std::to_string(data);
            }

        inline void serializerID::serializeData(dataBlock &block, const char *id, const char *data)
            {
                block.m_mappedData[id] = data;
            }

        inline void serializerID::serializeData(dataBlock &block, const char *id, char *data)
            {
                block.m_mappedData[id] = data;
            }

        template<typename T, typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::serializeData(dataBlock &block, const char *id, T &&data)
            {
                block.m_childDataBlocks.emplace_back(id, new dataBlock);
                data.serialize(*this, *block.m_childDataBlocks.back().second.get());
            }

        template<typename T, typename ...Args, typename std::enable_if<fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::serialize(dataBlock &block, const char *id, T &&dataVec, Args &&...args)
            {
                for (auto &data : dataVec)
                    {
                        serializeListData(block, id, data);
                    }
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename T, typename ...Args, typename std::enable_if<!fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type>
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

        template<typename T, typename std::enable_if<!fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::deserializeData(dataBlock &dataBlock, const char *id, T &newValue)
            {
                if (dataBlock.m_mappedData.find(id) != dataBlock.m_mappedData.end())
                    {
                        newValue = convertValue(dataBlock.m_mappedData[id], newValue);
                    }
            }

        template<typename T, typename std::enable_if<fe::is_vector<typename std::remove_reference<T>::type>::value, int>::type>
        void serializerID::deserializeData(dataBlock &dataBlock, const char *id, T &newValue)
            {
                if (dataBlock.m_mappedListPrimitiveData.find(id) != dataBlock.m_mappedListPrimitiveData.end())
                    {
                        for (auto &element : dataBlock.m_mappedListPrimitiveData[id])
                            {
                                newValue.emplace_back(convertValue<typename T::value_type>(element));
                            }
                    }
                else if (dataBlock.m_mappedListObjectData.find(id) != dataBlock.m_mappedListObjectData.end())
                    {
                        for (auto &element : dataBlock.m_mappedListObjectData[id])
                            {
                                newValue.emplace_back();
                                newValue.back().deserialize(*this, dataBlock.m_mappedListObjectData[id]);
                            }
                    }
            }

        inline void fe::serializerID::deserializeData(dataBlock &dataBlock, const char *id, char *newValue)
            {
                if (dataBlock.m_mappedData.find(id) != dataBlock.m_mappedData.end())
                    {
                        std::strcpy(newValue, convertValue(dataBlock.m_mappedData[id], newValue).c_str());
                    }
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

        template<typename T, typename ...Args>
        bool serializerID::deserializeBlock(std::vector<std::unique_ptr<dataBlock>> &blocks, const char*, const char *id, T &newValue, Args &&...args)
            {
                dataBlock *selectedBlock = nullptr;
                for (auto &datBlock : blocks)
                    {
                        selectedBlock = getDataBlock(datBlock.get());
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
void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); }\
bool deserialize(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { return serial.deserializeBlock(blocks, __VA_ARGS__); }

#define SERIALIZE_CALLBACK_ID(onSave, onLoad, ...) \
void serialize(fe::serializerID &serial) const { serial.serializeBlock(__VA_ARGS__); onSave; }\
bool deserialize(fe::serializerID &serial) { bool ret = serial.deserializeBlock(__VA_ARGS__); onLoad; return ret; }\
void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); onSave; }\
bool deserialize(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { bool ret = serial.deserializeBlock(blocks, __VA_ARGS__); onLoad; return ret;  }

#define SERIALIZE_NAME_ID(name, ...) \
void serialize##name(fe::serializerID &serial) const { serial.serializeBlock(__VA_ARGS__); }\
bool deserialize##name(fe::serializerID &serial) { return serial.deserializeBlock(__VA_ARGS__); }\
void serialize##name(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); }\
bool deserialize##name(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { return serial.deserializeBlock(blocks, __VA_ARGS__); }

#define SERIALIZE_NAME_CALLBACK_ID(onSave, onLoad,name, ...) \
void serialize##name(fe::serializerID &serial) const { serial.serializeBlock(__VA_ARGS__); onSave; }\
bool deserialize##name(fe::serializerID &serial) { bool ret = serial.deserializeBlock(__VA_ARGS__); onLoad; return ret; }\
void serialize##name(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); onSave; }\
bool deserialize##name(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { bool ret = serial.deserializeBlock(blocks, __VA_ARGS__); onLoad; return ret;  }
