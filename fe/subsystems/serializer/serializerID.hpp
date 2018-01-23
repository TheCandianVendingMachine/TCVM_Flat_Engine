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
                            std::unordered_map<std::string, std::unique_ptr<dataBlock>> m_mappedObjectData;
                            std::unordered_map<std::string, std::vector<std::unique_ptr<dataBlock>>> m_mappedListObjectData;

                            int m_size; // sizeof the datablock. Defaults to zero as for this is only used for lists

                            bool m_read; // if this block has already been read, we dont want to get data from it again
                            bool m_child;

                            dataBlock() : m_id(""), m_read(false), m_child(false), m_size(0) {}

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
                    template<typename T>
                    T convertValueTemplate(const std::string &in, T val = T()) { return convertValue(in, val); }

                    void serialize() {}
                    void serialize(dataBlock&) {}
                    void deserialize(dataBlock&) {}
                    void deserializeData(dataBlock&, const char*) {}
                    

                    template<typename T>
                    void serializeListData(dataBlock &block, const char *blockID, T &&data);


                    template<typename T, typename std::enable_if<!std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeData(dataBlock &block, const char *id, T &&data);

                    void serializeData(dataBlock &block, const char *id, const char *data);
                    void serializeData(dataBlock &block, const char *id, char *data);

                    template<typename T, typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value, int>::type = 0>
                    void serializeData(dataBlock &block, const char *id, T &&data);


                    template<typename T, typename std::enable_if<std::is_array<T>::value, int>::type = 0>
                    void deserializeArrayHelper(dataBlock &dataBlock, const char *id, T &array);
                    template<typename T, typename std::enable_if<!std::is_array<T>::value, int>::type = 0>
                    void deserializeArrayHelper(dataBlock &dataBlock, const char *id, T &array);

                    template<typename T, typename ...Args>
                    void serialize(dataBlock &block, const char *id, T &&dataAppend, Args &&...args);
                    template<typename ...Args>
                    void serialize(dataBlock &block, const char *id, const char *dataAppend, Args &&...args);

                    template<typename T>
                    void deserializeData(dataBlock &dataBlock, const char *id, T &newValue);

                    void deserializeData(dataBlock &dataBlock, const char *id, char *newValue);

                    template<typename T, typename ...Args>
                    void deserialize(dataBlock &dataBlock, const char *id, T &newValue, Args &&...args);


                    FLAT_ENGINE_API dataBlock *getDataBlock(dataBlock *initial, const char *id);
                    FLAT_ENGINE_API dataBlock *getDataBlock(dataBlock *initial);
                    FLAT_ENGINE_API void interpretData(const char *dataBlock);

                public:
                    template<typename ...Args>
                    int serializeBlock(const char *blockID, Args &&...args);
                    template<typename ...Args>
                    void serializeBlock(dataBlock &block, const char *blockID, Args &&...Args);
                    template<typename ...Args>
                    void serializeChildBlock(dataBlock &block, const char *blockID, const char *oldID, Args &&...Args);
                    template<typename T, typename ...Args>
                    int deserializeBlock(const char *blockID, const char *id, T &newValue, Args &&...args);
                    template<typename T, typename ...Args>
                    bool deserializeBlock(std::vector<std::unique_ptr<dataBlock>> &blocks, const char*, const char *id, T &newValue, Args &&...args);
                    template<typename T, typename ...Args>
                    bool deserializeBlock(std::unique_ptr<dataBlock> &block, const char*, const char *id, T &newValue, Args &&...args);

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

                    // Append data block to another
                    FLAT_ENGINE_API void appendTo(int i1, int i2);

                    FLAT_ENGINE_API std::unique_ptr<fe::serializerID::dataBlock> &getDataBlock(int index);

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
                return in;
            }

        inline std::string serializerID::convertValue(const std::string &in, char *val)
            {
                return in;
            }

        template<typename T>
        void serializerID::serializeListData(dataBlock &block, const char *blockID, T &&data)
            {
                bool constexpr isPtr = (std::is_pointer<typename std::remove_reference<T>::type>::value);
                if constexpr (!std::is_class<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::value)
                    {
                        if constexpr (isPtr)
                            {
                                block.m_mappedListPrimitiveData[blockID].push_back(std::to_string(*data));
                            }
                        else
                            {
                                block.m_mappedListPrimitiveData[blockID].push_back(std::to_string(data));
                            }
                    }
                else if constexpr (std::is_class<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::value)
                    {
                        block.m_mappedListObjectData[blockID].emplace_back(new dataBlock);
                        block.m_mappedListObjectData[blockID].back()->m_id = blockID;
                        if constexpr (isPtr)
                            {
                                data->serialize(*this, *block.m_mappedListObjectData[blockID].back().get());
                            }
                        else
                            {
                                data.serialize(*this, *block.m_mappedListObjectData[blockID].back().get());
                            }
                    }
                block.m_size++;
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
                block.m_mappedObjectData[id].reset(new dataBlock);
                data.serialize(*this, *block.m_mappedObjectData[id].get(), id);
            }

        template<typename T, typename ...Args>
        void serializerID::serialize(dataBlock &block, const char *id, T &&dataAppend, Args &&...args)
            {
                if constexpr (fe::is_vector<typename std::remove_reference<T>::type>::value)
                    {
                        for (auto &data : dataAppend)
                            {
                                serializeListData(block, id, data);
                            }
                    }
                else if constexpr (!fe::is_vector<typename std::remove_reference<T>::type>::value && !std::is_array<typename std::remove_reference<T>::type>::value)
                    {
                        serializeData(block, id, std::forward<T>(dataAppend));
                    }
                else if constexpr (std::is_array<typename std::remove_reference<T>::type>::value)
                    {
                        for (unsigned int i = 0; i < sizeof(dataAppend) / sizeof(*dataAppend); i++)
                            {
                                if (dataAppend[i])
                                    {
                                        serializeListData(block, id, dataAppend[i]);
                                    }
                            }
                    }
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename ...Args>
        void serializerID::serialize(dataBlock &block, const char *id, const char *dataAppend, Args &&...args)
            {
                serializeData(block, id, dataAppend);
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename ...Args>
        void fe::serializerID::serializeBlock(dataBlock &block, const char *blockID, Args &&...args)
            {
                block.m_id = blockID;
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename ...Args>
        void serializerID::serializeChildBlock(dataBlock &block, const char *blockID, const char *oldID, Args &&...args)
            {
                char newID[512] = "\0";
                std::strcpy(newID, blockID);
                newID[std::strlen(blockID)] = ':';
                std::strcpy(newID + 1 + std::strlen(blockID), oldID);
                
                block.m_id = newID;
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename ...Args>
        int serializerID::serializeBlock(const char *blockID, Args &&...args)
            {
                m_data.emplace_back(new dataBlock);
                dataBlock *block = m_data.back().get();
                block->m_id = blockID;
                serialize(*block, std::forward<Args>(args)...);
                return static_cast<int>(m_data.size() - 1);
            }

        template<typename T, typename std::enable_if<std::is_array<T>::value, int>::type>
        void serializerID::deserializeArrayHelper(dataBlock &dataBlock, const char *id, T &array)
            {
                constexpr bool isPtr = std::is_pointer<typename std::remove_reference<decltype(*array)>::type>::value;
                if (dataBlock.m_mappedListPrimitiveData.find(id) != dataBlock.m_mappedListPrimitiveData.end())
                    {
                        FE_LOG_ERROR("Fuck you primitive data (Sorry future me for not implementing this)");
                    }
                else if (dataBlock.m_mappedListObjectData.find(id) != dataBlock.m_mappedListObjectData.end())
                    {
                        for (int i = 0; i < dataBlock.m_size; i++)
                            {
                                if constexpr (isPtr)
                                    {
                                        array[i] = new (typename std::remove_pointer<typename std::remove_reference<decltype(*array)>::type>::type)();
                                        array[i]->deserialize(*this, dataBlock.m_mappedListObjectData[id]);
                                    }
                                else if constexpr (!isPtr)
                                    {
                                        array[i] = typename std::remove_reference<decltype(*array)>::type();
                                        array[i].deserialize(*this, dataBlock.m_mappedListObjectData[id]);
                                    }
                            }
                    }
                else
                    {
                        FE_LOG_ERROR("Cannot load array from serializer with id: ", id);
                    }
            }

        template<typename T, typename std::enable_if<!std::is_array<T>::value, int>::type>
        void serializerID::deserializeArrayHelper(dataBlock &dataBlock, const char *id, T &array)
            {
                // Empty function for non-array array's.
                // See deserializeData for explaination
            }

        template<typename T>
        void serializerID::deserializeData(dataBlock &dataBlock, const char *id, T &newValue)
            {
                if constexpr (fe::is_vector<typename std::remove_reference<T>::type>::value)
                    {
                        if (dataBlock.m_mappedListPrimitiveData.find(id) != dataBlock.m_mappedListPrimitiveData.end())
                            {
                                for (auto &element : dataBlock.m_mappedListPrimitiveData[id])
                                    {
                                        newValue.emplace_back(convertValueTemplate<typename T::value_type>(element));
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
                        else
                            {
                                FE_LOG_ERROR("Cannot load std::vector from serializer with id: ", id);
                            }
                    }
                else if constexpr (!fe::is_vector<typename std::remove_reference<T>::type>::value && !std::is_array<typename std::remove_reference<T>::type>::value)
                    {
                        if (dataBlock.m_mappedData.find(id) != dataBlock.m_mappedData.end())
                            {
                                newValue = convertValue(dataBlock.m_mappedData[id], newValue);
                            }
                        else if (dataBlock.m_mappedObjectData.find(id) != dataBlock.m_mappedObjectData.end())
                            {
                                if constexpr (std::is_class<T>::value && std::is_pointer<typename std::remove_reference<T>::type>::value)
                                    {
                                        newValue = new T();
                                        newValue->deserialize(*this, dataBlock.m_mappedObjectData[id]);
                                    }
                                else if constexpr (std::is_class<T>::value && !std::is_pointer<typename std::remove_reference<T>::type>::value)
                                    {
                                        newValue = T();
                                        newValue.deserialize(*this, dataBlock.m_mappedObjectData[id]);
                                    }
                            }
                        else
                            {
                                FE_LOG_ERROR("Cannot load data from serializer with id: ", id);
                            }
                    }
                else if constexpr (std::is_array<typename std::remove_reference<T>::type>::value)
                    {
                        // Because apparently MSVC compiles this for non-array types we need a helper to distinguish between array and non array's
                        deserializeArrayHelper(dataBlock, id, newValue);
                    }
                else
                    {
                        FE_LOG_ERROR("Unknown Error with id: ", id);
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
        int serializerID::deserializeBlock(const char *blockID, const char *id, T &newValue, Args &&...args)
            {
                dataBlock *selectedBlock = nullptr;
                int index = 0;
                for (auto &datBlock : m_data)
                    {
                        ++index;
                        selectedBlock = getDataBlock(datBlock.get(), blockID);
                        if (selectedBlock) break;
                    }

                if (selectedBlock) 
                    {
                        selectedBlock->m_read = true;
                        deserialize(*selectedBlock, id, newValue, args...);
                    }
                return index - 1;
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
        template<typename T, typename ...Args>
        bool serializerID::deserializeBlock(std::unique_ptr<dataBlock> &block, const char*, const char *id, T &newValue, Args &&...args)
            {
                block->m_read = true;
                deserialize(*block.get(), id, newValue, args...);
                return true;
            }
    }

#define SERIALIZE_ID(...) \
virtual int serialize(fe::serializerID &serial) const { return serial.serializeBlock(__VA_ARGS__); }\
virtual bool deserialize(fe::serializerID &serial) { return serial.deserializeBlock(__VA_ARGS__); }\
virtual void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); }\
virtual bool deserialize(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { return serial.deserializeBlock(blocks, __VA_ARGS__); }\
virtual void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block, const char *blockID) const { serial.serializeChildBlock(block, blockID, __VA_ARGS__); }\
virtual bool deserialize(fe::serializerID &serial, std::unique_ptr<fe::serializerID::dataBlock> &block) { return serial.deserializeBlock(block, __VA_ARGS__); }

#define SERIALIZE_CALLBACK_ID(onSave, onLoad, ...) \
virtual int serialize(fe::serializerID &serial) const { auto ret = serial.serializeBlock(__VA_ARGS__); onSave(serial); return ret; }\
virtual bool deserialize(fe::serializerID &serial) { bool ret = serial.deserializeBlock(__VA_ARGS__); onLoad(serial); return ret; }\
virtual void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); onSave(serial); }\
virtual bool deserialize(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { bool ret = serial.deserializeBlock(blocks, __VA_ARGS__); onLoad(serial); return ret;  }\
virtual void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block, const char *blockID) const { serial.serializeChildBlock(block, blockID, __VA_ARGS__); onSave(serial); }\
virtual bool deserialize(fe::serializerID &serial, std::unique_ptr<fe::serializerID::dataBlock> &block) { bool ret = serial.deserializeBlock(block, __VA_ARGS__); onLoad(serial); return ret; }

#define SERIALIZE_NAME_ID(name, ...) \
virtual int serialize##name(fe::serializerID &serial) const { return serial.serializeBlock(__VA_ARGS__); }\
virtual bool deserialize##name(fe::serializerID &serial) { return serial.deserializeBlock(__VA_ARGS__); }\
virtual void serialize##name(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); }\
virtual bool deserialize##name(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { return serial.deserializeBlock(blocks, __VA_ARGS__); }\
virtual void serialize##name(fe::serializerID &serial, fe::serializerID::dataBlock &block, const char *blockID) const { serial.serializeChildBlock(block, blockID, __VA_ARGS__); }\
virtual bool deserialize##name(fe::serializerID &serial, std::unique_ptr<fe::serializerID::dataBlock> &block) { return serial.deserializeBlock(block, __VA_ARGS__); }

#define SERIALIZE_NAME_CALLBACK_ID(onSave, onLoad,name, ...) \
virtual int serialize##name(fe::serializerID &serial) const { auto ret = serial.serializeBlock(__VA_ARGS__); onSave(serial); return ret; }\
virtual bool deserialize##name(fe::serializerID &serial) { bool ret = serial.deserializeBlock(__VA_ARGS__); onLoad(serial); return ret; }\
virtual void serialize##name(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__); onSave(serial); }\
virtual bool deserialize##name(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { bool ret = serial.deserializeBlock(blocks, __VA_ARGS__); onLoad(serial); return ret;  }\
virtual void serialize##name(fe::serializerID &serial, fe::serializerID::dataBlock &block, const char *blockID) const { serial.serializeChildBlock(block, blockID, __VA_ARGS__); onSave(serial); }\
virtual bool deserialize##name(fe::serializerID &serial, std::unique_ptr<fe::serializerID::dataBlock> &block) { bool ret = serial.deserializeBlock(block, __VA_ARGS__); onLoad(serial); return ret; }

#define SERIALIZE_PARENT_ID(parentSerialize, parentDeserialize, ...)\
virtual int serialize(fe::serializerID &serial) const { auto ret = serial.serializeBlock(__VA_ARGS__); auto b = parentSerialize(serial); serial.appendTo(ret, b); return ret; }\
virtual bool deserialize(fe::serializerID &serial) { auto ret = serial.deserializeBlock(__VA_ARGS__); parentDeserialize(serial, serial.getDataBlock(ret)); return ret; }\
virtual void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block) const { serial.serializeBlock(block, __VA_ARGS__);  }\
virtual bool deserialize(fe::serializerID &serial, std::vector<std::unique_ptr<fe::serializerID::dataBlock>> &blocks) { return serial.deserializeBlock(blocks, __VA_ARGS__); }\
virtual void serialize(fe::serializerID &serial, fe::serializerID::dataBlock &block, const char *blockID) const { serial.serializeChildBlock(block, blockID, __VA_ARGS__);  }\
virtual bool deserialize(fe::serializerID &serial, std::unique_ptr<fe::serializerID::dataBlock> &block) { return serial.deserializeBlock(block, __VA_ARGS__); }
