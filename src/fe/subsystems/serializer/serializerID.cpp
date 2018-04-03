#include "fe/subsystems/serializer/serializerID.hpp"
#include "fe/subsystems/serializer/serializable.hpp"
#include <string.h>

#ifdef FE_IS_ENGINE
    #include "fe/feAssert.hpp"
#else
    #define FE_ASSERT(check, message) assert(check && message)
#endif

void fe::serializerID::dataBlock::outData(std::ostream &out, const char *preDataText)
    {
        out << preDataText << "obj " << m_id << "{\n";
        for (auto &dat : m_mappedData)
            {
                out << preDataText << "\tdat " << dat.first << ":" << dat.second << ";\n";
            }

        char preDataTextApp[512] = "\0";
        std::strcat(preDataTextApp, preDataText);
        std::strcat(preDataTextApp, "\t");

        for (auto &listDat : m_mappedListPrimitiveData)
            {
                out << preDataText << "\tils " << listDat.first << "(" << m_size << "):[\n";
                for (auto &item : listDat.second)
                    {
                        out << preDataTextApp << "\titm " << item << ";\n";
                    }
                out << preDataText << "\t]\n";
            }

        for (auto &memberDat : m_mappedObjectData)
            {
                memberDat.second->outData(out, preDataTextApp);
            }

        std::strcat(preDataTextApp, "\t");
        for (auto &listDat : m_mappedListObjectData)
            {
                out << preDataText << "\tols " << listDat.first << "(" << m_size << "):[\n";
                for (auto &item : listDat.second)
                    {
                        item->outData(out, preDataTextApp);
                    }
                out << preDataText << "\t]\n";
            }

        out << preDataText << "}\n";
    }

void fe::serializerID::dataBlock::readData(const char *block)
    {
        std::string id;
        std::string dat;
        bool readingID = true;
        for (unsigned int i = 0; i < std::strlen(block); i++)
            {
                if (block[i] == ':')
                    {
                        readingID = false;
                    }
                else if (readingID)
                    {
                        id += block[i];
                    }
                else
                    {
                        dat += block[i];
                    }
            }
        m_mappedData[id] = dat;
    }

bool fe::serializerID::dataBlock::hasData(const char *dataId)
    {
        if (m_id == dataId) return true;
        for (auto &childObject : m_mappedObjectData)
            {
                if (childObject.second->hasData(dataId)) return true;
            }

        return false;
    }

fe::serializerID::dataBlock::~dataBlock()
    {
        for (auto &pair : m_mappedListObjectData)
            {
                for (auto &uPtr : pair.second)
                    {
                        delete uPtr.release();
                    }
            }

        for (auto &pair : m_mappedObjectData)
            {
                delete pair.second.release();
            }
    }

fe::serializerID::dataBlock &fe::serializerID::dataBlock::operator=(fe::serializerID::dataBlock &rhs)
    {
        if (&rhs != this)
            {
                m_id = rhs.m_id;
                m_mappedData.merge(rhs.m_mappedData);
                m_mappedListPrimitiveData.merge(rhs.m_mappedListPrimitiveData);
                m_mappedObjectData.merge(rhs.m_mappedObjectData);
                m_mappedListObjectData.merge(rhs.m_mappedListObjectData);

                m_size = rhs.m_size;
            }

        return *this;
    }

void fe::serializerID::interpretData(const char *block)
    {
        enum readState
            {
                OBJ_START,
                OBJ_END,
                OBJ_NAME_START,
                OBJ_NAME_END,
                DAT_START,
                DAT_END,
                PRIMITIVE_LIST_START,
                OBJECT_LIST_START,
                LIST_NAME_START,
                LIST_NAME_END,
                LIST_ITEM_START,
                LIST_ITEM_END,
                LIST_END,
                LIST_SIZE_END
            };
        std::stack<std::pair<readState, unsigned int>> readingState;
        std::stack<readState> lastDataReadState;

        for (unsigned int i = 0; i < std::strlen(block); i++)
            {
                if (i + 3 < std::strlen(block) && block[i + 3] == ' ')
                    {
                        if (block[i + 0] == 'o' && block[i + 1] == 'b' && block[i + 2] == 'j')
                            {
                                readingState.push(std::make_pair(OBJ_START, i));
                                readingState.push(std::make_pair(OBJ_NAME_START, i + 4));
                            }
                        else if (block[i + 0] == 'd' && block[i + 1] == 'a' && block[i + 2] == 't')
                            {
                                readingState.push(std::make_pair(DAT_START, i));
                                lastDataReadState.push(DAT_START);
                            }
                        else if (block[i + 0] == 'i' && block[i + 1] == 'l' && block[i + 2] == 's')
                            {
                                readingState.push(std::make_pair(PRIMITIVE_LIST_START, i));
                                readingState.push(std::make_pair(LIST_NAME_START, i));
                                lastDataReadState.push(PRIMITIVE_LIST_START);
                            }
                        else if (block[i + 0] == 'o' && block[i + 1] == 'l' && block[i + 2] == 's')
                            {
                                readingState.push(std::make_pair(OBJECT_LIST_START, i));
                                readingState.push(std::make_pair(LIST_NAME_START, i));
                                lastDataReadState.push(OBJECT_LIST_START);
                            }
                        else if (block[i + 0] == 'i' && block[i + 1] == 't' && block[i + 2] == 'm')
                            {
                                readingState.push(std::make_pair(LIST_ITEM_START, i + 4));
                            }
                    }

                if (block[i] == '}')
                    {
                        readingState.push(std::make_pair(OBJ_END, i));
                    }
                else if (block[i] == '{')
                    {
                        readingState.push(std::make_pair(OBJ_NAME_END, i));
                    }
                else if (block[i] == ']')
                    {
                        readingState.push(std::make_pair(LIST_END, i));
                        lastDataReadState.pop();
                    }
                else if (block[i] == '(')
                    {
                        readingState.push(std::make_pair(LIST_NAME_END, i + 1));
                    }
                else if (block[i] == ')')
                    {
                        readingState.push(std::make_pair(LIST_SIZE_END, i));
                    }
                else if (block[i] == ';')
                    {
                        if (lastDataReadState.top() == DAT_START)
                            {
                                readingState.push(std::make_pair(DAT_END, i));
                                lastDataReadState.pop();
                            }
                        else if (lastDataReadState.top() == PRIMITIVE_LIST_START || lastDataReadState.top() == OBJECT_LIST_START)
                            {
                                readingState.push(std::make_pair(LIST_ITEM_END, i));
                            }
                    }
            }

        std::stack<std::pair<readState, unsigned int>> reverseReadingStack;
        while (!readingState.empty())
            {
                reverseReadingStack.push(readingState.top());
                readingState.pop();
            }

        enum readingStorage
            {
                NONE,
                LIST,
            };

        std::stack<readingStorage> currentStorageRead;
        currentStorageRead.push(NONE);

        std::stack<std::unique_ptr<dataBlock>> currentBlock;
        std::stack<std::string> currentListID;
        std::stack<int> currentSize;
        while (!reverseReadingStack.empty())
            {
                switch (reverseReadingStack.top().first)
                    {
                        case OBJ_START:
                            currentBlock.emplace(new dataBlock);
                            currentStorageRead.push(NONE);
                            break;
                        case OBJ_END:
                            {
                                currentStorageRead.pop();
                                dataBlock *top = currentBlock.top().release();
                                currentBlock.pop();
                                if (!currentBlock.empty()) 
                                    {
                                        switch (currentStorageRead.top())
                                            {
                                                case NONE:
                                                    currentBlock.top()->m_mappedObjectData[top->m_id].reset(top);
                                                    break;
                                                case LIST:
                                                    currentBlock.top()->m_mappedListObjectData[currentListID.top()].emplace_back(top);
                                                    // Since "List" is a misnomer and actually could mean list-object data we have to check
                                                    // if this is empty to avoid a access error
                                                    if (!currentSize.empty()) 
                                                        {
                                                        currentBlock.top()->m_size = currentSize.top();
                                                            currentSize.pop();
                                                        }
                                                    break;
                                                default:
                                                    break;
                                            }
                                    }
                                else
                                    {
                                        *m_baseDataBlock.get() = *top;
                                    }
                            }
                            break;
                        case OBJ_NAME_START:
                            {
                                unsigned int startIndex = reverseReadingStack.top().second;
                                reverseReadingStack.pop();
                                unsigned int endIndex = reverseReadingStack.top().second;

                                bool hasAlias = false;
                                char idName[512] = "\0";
                                for (unsigned int i = startIndex; i < endIndex; i++)
                                    {
                                        if (block[i] == ':')
                                            {
                                                hasAlias = true;
                                                startIndex = i + 1;
                                                break;
                                            }
                                        idName[i - startIndex] = block[i];
                                    }

                                currentBlock.top()->m_id = idName;
                            }
                            break;
                        case OBJ_NAME_END:
                            break;
                        case DAT_START:
                            {
                                unsigned int startIndex = reverseReadingStack.top().second + 4;
                                reverseReadingStack.pop();
                                unsigned int endIndex = reverseReadingStack.top().second;

                                char data[512] = "\0";
                                std::memcpy(data, block + startIndex, endIndex - startIndex);
                                currentBlock.top()->readData(data);
                            }
                            break;
                        case DAT_END:
                            break;
                        case PRIMITIVE_LIST_START:
                            break;
                        case OBJECT_LIST_START:
                            currentStorageRead.push(LIST);
                            break;
                        case LIST_END:
                            currentListID.pop();
                            //currentStorageRead.pop();
                            break;
                        case LIST_NAME_START:
                            {
                                int nameStart = reverseReadingStack.top().second + 4;
                                reverseReadingStack.pop();
                                int nameEnd = reverseReadingStack.top().second - 1;

                                char name[512] = "\0";
                                std::memcpy(name, block + nameStart, nameEnd - nameStart);
                                currentListID.push(name);

                                // Get the list size
                                int sizeStart = reverseReadingStack.top().second;
                                reverseReadingStack.pop();
                                int sizeEnd = reverseReadingStack.top().second;
                                char size[512] = "\0";
                                std::memcpy(size, block + sizeStart, sizeEnd - sizeStart);
                                currentSize.push(std::stoi(size));
                            }
                            break;
                        case LIST_ITEM_START:
                            {
                                int itemStart = reverseReadingStack.top().second;
                                reverseReadingStack.pop();
                                int itemEnd = reverseReadingStack.top().second;

                                char data[512] = "\0";
                                std::memcpy(data, block + itemStart, itemEnd - itemStart);
                                currentBlock.top()->m_mappedListPrimitiveData[currentListID.top()].push_back(data);
                            }
                            break;
                        case LIST_ITEM_END:
                            break;
                        default:
                            break;
                    }
                reverseReadingStack.pop();
            }
    }

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

fe::serializerID::serializerID() : m_baseDataBlock(new dataBlock("serialized_items"))
    {
        m_currentBlock.push(m_baseDataBlock.get());
    }

void fe::serializerID::writeObject(const std::string &id, const serializable &data)
    {
        std::unique_ptr<dataBlock> *currentBlock = nullptr;
        FE_ASSERT(!m_currentBlock.empty(), "Current block is empty. Should never happen if fe::serializerID::serializerID [constructor] is called");
        m_currentBlock.top()->m_mappedObjectData[id].reset(new dataBlock(id));
        currentBlock = &m_currentBlock.top()->m_mappedObjectData[id];
        m_currentBlock.push(currentBlock->get());
        data.serialize(*this);
        m_currentBlock.pop();
    }

void fe::serializerID::writeObjectList(const std::string &id, const serializable &data)
    {
        std::unique_ptr<dataBlock> *currentBlock = nullptr;
        FE_ASSERT(!m_currentBlock.empty(), "Current block is empty. Should never happen if fe::serializerID::serializerID [constructor] is called");
        m_currentBlock.top()->m_mappedListObjectData[id].emplace_back(new dataBlock(id));
        currentBlock = &m_currentBlock.top()->m_mappedListObjectData[id].back();
        m_currentBlock.push(currentBlock->get());
        data.serialize(*this);
        m_currentBlock.pop();
    }

void fe::serializerID::readObject(const std::string &id, serializable &data)
    {
        if (m_currentBlock.top()->m_mappedObjectData.find(id) != m_currentBlock.top()->m_mappedObjectData.end())
            {
                m_currentBlock.push(m_currentBlock.top()->m_mappedObjectData[id].get());
                data.deserialize(*this);
                m_currentBlock.pop();
            }
    }

void fe::serializerID::readObjectList(const std::string &id, serializable &data)
    {
        if (m_currentBlock.top()->m_mappedListObjectData.find(id) != m_currentBlock.top()->m_mappedListObjectData.end())
            {
                m_currentBlock.push(m_currentBlock.top()->m_mappedListObjectData[id].back().release());
                data.deserialize(*this);
                m_currentBlock.pop();

                m_currentBlock.top()->m_mappedListObjectData[id].erase(m_currentBlock.top()->m_mappedListObjectData[id].end() - 1);
            }
    }

bool fe::serializerID::listHasItems(const std::string &listID)
    {
        if (m_currentBlock.top()->m_mappedListObjectData.find(listID) != m_currentBlock.top()->m_mappedListObjectData.end())
            {
                return !m_currentBlock.top()->m_mappedListObjectData[listID].empty();
            }
        else if (m_currentBlock.top()->m_mappedListPrimitiveData.find(listID) != m_currentBlock.top()->m_mappedListPrimitiveData.end())
            {
                return !m_currentBlock.top()->m_mappedListPrimitiveData[listID].empty();
            }
        return false;
    }

void fe::serializerID::clearData()
    {
        m_baseDataBlock->m_mappedData.clear();
        m_baseDataBlock->m_mappedListObjectData.clear();
        m_baseDataBlock->m_mappedListPrimitiveData.clear();
        m_baseDataBlock->m_mappedObjectData.clear();

        m_baseDataBlock->m_size = 0;
    }

void fe::serializerID::outData(std::ostream &out)
    {
        m_baseDataBlock->outData(out);
    }

void fe::serializerID::readData(std::istream &in)
    {
        clearData();

        std::string datBlock;
        std::string line;
        while (std::getline(in, line))
            {
                datBlock += line;
            }

        interpretData(datBlock.c_str());
    }

fe::serializerID::~serializerID()
    {
        clearData();
    }
