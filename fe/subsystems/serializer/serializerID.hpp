// serializerID
// Allows the user to serialize data to a stream with IDs to
// get and set data easily
#pragma once
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>

namespace fe
    {
        class serializerID
            {
                protected:
                    struct dataBlock 
                        {
                            std::string m_id;
                            std::unordered_map<std::string, std::string> m_mappedData;
                            bool m_read; // if this block has already been read, we dont want
                                         // to get data from it again

                            dataBlock() : m_id(""), m_read(false) {}

                            void outData(std::ostream &out);
                            void readData(const std::string &block);
                        };

                    std::vector<dataBlock> m_data;

                    template<typename T>
                    T convertValue(const std::string &in, T val) { return T(); }
                    template<>
                    int convertValue(const std::string &in, int val);
                    template<>
                    unsigned int convertValue(const std::string &in, unsigned int val);
                    template<>
                    float convertValue(const std::string &in, float val);
                    template<>
                    double convertValue(const std::string &in, double val);
                    template<>
                    bool convertValue(const std::string &in, bool val);

                public:
                    void serialize() 
                        {}

                    template<typename T>
                    void serialize(dataBlock &block, const std::string &id, T &&data);

                    template<typename T, typename ...Args>
                    void serialize(dataBlock &block, const std::string &id, T &&data, Args &&...args);

                    template<typename ...Args>
                    void serializeBlock(const std::string &blockID, Args &&...args);

                    void deserialize() {}
                    template<typename T>
                    void deserialize(dataBlock &dataBlock, const std::string &id, T &newValue);

                    template<typename T, typename ...Args>
                    void deserialize(dataBlock &dataBlock, const std::string &id, T &newValue, Args &&...args);

                    template<typename T, typename ...Args>
                    void deserializeBlock(const std::string &blockID, const std::string &id, T &newValue, Args &&...args);

                    void outData(std::ostream &out)
                        {
                            for (auto &data : m_data)
                                {
                                    data.outData(out);
                                }
                        }

                    void readData(std::istream &in);
            };

        template<>
        inline int serializerID::convertValue(const std::string &in, int val)
            {
                return std::stoi(in);
            }

        template<>
        inline unsigned int serializerID::convertValue(const std::string &in, unsigned int val)
            {
                return std::stoul(in);
            }

        template<>
        inline float serializerID::convertValue(const std::string &in, float val)
            {
                return std::stof(in);
            }

        template<>
        inline double serializerID::convertValue(const std::string &in, double val)
            {
                return std::stod(in);
            }

        template<>
        inline bool serializerID::convertValue(const std::string &in, bool val)
            {
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

        template<typename T>
        void serializerID::serialize(dataBlock &block, const std::string &id, T &&data)
            {
                block.m_mappedData[id] = std::to_string(data);
            }

        template<typename T, typename ...Args>
        void serializerID::serialize(dataBlock &block, const std::string &id, T &&data, Args &&...args)
            {
                serialize(block, id, data);
                serialize(block, std::forward<Args>(args)...);
            }

        template<typename ...Args>
        void serializerID::serializeBlock(const std::string &blockID, Args &&...args)
            {
                dataBlock block;
                block.m_id = blockID;
                serialize(block, args...);
                m_data.push_back(block);
            }

        template<typename T>
        void serializerID::deserialize(dataBlock &dataBlock, const std::string &id, T &newValue)
            {
                newValue = convertValue(dataBlock.m_mappedData[id], newValue);
                int i = 0;
            }

        template<typename T, typename ...Args>
        void serializerID::deserialize(dataBlock &dataBlock, const std::string &id, T &newValue, Args &&...args)
            {
                deserialize(dataBlock, id, newValue);
                deserialize(dataBlock, std::forward<Args>(args)...);
            }

        template<typename T, typename ...Args>
        void serializerID::deserializeBlock(const std::string &blockID, const std::string &id, T &newValue, Args &&...args)
            {
                dataBlock *selectedBlock = nullptr;
                for (auto &datBlock : m_data)
                    {
                        if (datBlock.m_id == blockID && !datBlock.m_read)
                            {
                                selectedBlock = &datBlock;
                            }
                    }

                if (selectedBlock) 
                    {
                        selectedBlock->m_read = true;
                        deserialize(*selectedBlock, id, newValue, args...);
                    }
            }

    }

#define SERIALIZE_ID(...) \
void serialize(fe::serializerID &serial) { serial.serializeBlock(__VA_ARGS__); }\
void deserialize(fe::serializerID &serial) { serial.deserializeBlock(__VA_ARGS__); }