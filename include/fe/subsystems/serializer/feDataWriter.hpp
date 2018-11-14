// feDataWriter.hpp
// a serializer format that is a custom data type
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/serializer/dataWriter.hpp"

#include <stack>
#include <unordered_map>

namespace fe
    {
        class feDataWriter : public fe::dataWriter
            {
                private:
                    struct dataBlock 
                        {
                            std::string m_title;
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

                    const std::unique_ptr<dataBlock> m_baseDataBlock;
                    std::stack<dataBlock*> m_currentBlock; // current block we are writing to

                    bool m_firstRead;
                    std::vector<std::unique_ptr<dataBlock>> *m_wantedData;

                    FLAT_ENGINE_API void interpretData(const char *dataBlock);

                public:
                    FLAT_ENGINE_API feDataWriter();
                    FLAT_ENGINE_API ~feDataWriter();

                    FLAT_ENGINE_API void setTitle(const std::string &title);

                    FLAT_ENGINE_API void startObject(const std::string &id);
                    FLAT_ENGINE_API void startObjectList(const std::string &id);
                    FLAT_ENGINE_API void startObjectRead(const std::string &id);
                    FLAT_ENGINE_API void startObjectListRead(const std::string &id);
                    FLAT_ENGINE_API void endObject();

                    FLAT_ENGINE_API void write(const std::string &id, const std::string &value);
                    FLAT_ENGINE_API void writeList(const std::string &id, const std::string &value);

                    FLAT_ENGINE_API std::string read(const std::string &id);
                    FLAT_ENGINE_API std::string readList(const std::string &id);

                    FLAT_ENGINE_API void outData(std::ostream &out);
                    FLAT_ENGINE_API void readData(std::istream &in);

                    FLAT_ENGINE_API bool listHasItems(const std::string &list);

                    FLAT_ENGINE_API void clearData();
            };
    }
