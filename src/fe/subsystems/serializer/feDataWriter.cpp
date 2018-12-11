#include "fe/subsystems/serializer/feDataWriter.hpp"
#include "fe/subsystems/serializer/serializable.hpp"
#include "fe/feAssert.hpp"
#include <memory>
#include <algorithm>
#include <cstring>

void fe::feDataWriter::dataBlock::outData(std::ostream &out, const char *preDataText)
    {
        out << preDataText << "obj " << m_id;
        if (!m_title.empty())
            {
                out << ":" << m_title;
            }
        out << "{\n";
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

void fe::feDataWriter::dataBlock::readData(const char *block)
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

bool fe::feDataWriter::dataBlock::hasData(const char *dataId)
    {
        if (m_id == dataId) return true;
        for (auto &childObject : m_mappedObjectData)
            {
                if (childObject.second->hasData(dataId)) return true;
            }

        return false;
    }

fe::feDataWriter::dataBlock::~dataBlock()
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

fe::feDataWriter::dataBlock &fe::feDataWriter::dataBlock::operator=(fe::feDataWriter::dataBlock &rhs)
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

void fe::feDataWriter::interpretData(const char *dataBlock)
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

            for (unsigned int i = 0; i < std::strlen(dataBlock); i++)
                {
                    if (i + 3 < std::strlen(dataBlock) && dataBlock[i + 3] == ' ')
                        {
                            if (dataBlock[i + 0] == 'o' && dataBlock[i + 1] == 'b' && dataBlock[i + 2] == 'j')
                                {
                                    readingState.push(std::make_pair(OBJ_START, i));
                                    readingState.push(std::make_pair(OBJ_NAME_START, i + 4));
                                }
                            else if (dataBlock[i + 0] == 'd' && dataBlock[i + 1] == 'a' && dataBlock[i + 2] == 't')
                                {
                                    readingState.push(std::make_pair(DAT_START, i));
                                    lastDataReadState.push(DAT_START);
                                }
                            else if (dataBlock[i + 0] == 'i' && dataBlock[i + 1] == 'l' && dataBlock[i + 2] == 's')
                                {
                                    readingState.push(std::make_pair(PRIMITIVE_LIST_START, i));
                                    readingState.push(std::make_pair(LIST_NAME_START, i));
                                    lastDataReadState.push(PRIMITIVE_LIST_START);
                                }
                            else if (dataBlock[i + 0] == 'o' && dataBlock[i + 1] == 'l' && dataBlock[i + 2] == 's')
                                {
                                    readingState.push(std::make_pair(OBJECT_LIST_START, i));
                                    readingState.push(std::make_pair(LIST_NAME_START, i));
                                    lastDataReadState.push(OBJECT_LIST_START);
                                }
                            else if (dataBlock[i + 0] == 'i' && dataBlock[i + 1] == 't' && dataBlock[i + 2] == 'm')
                                {
                                    readingState.push(std::make_pair(LIST_ITEM_START, i + 4));
                                }
                        }

                    if (dataBlock[i] == '}')
                        {
                            readingState.push(std::make_pair(OBJ_END, i));
                        }
                    else if (dataBlock[i] == '{')
                        {
                            readingState.push(std::make_pair(OBJ_NAME_END, i));
                        }
                    else if (dataBlock[i] == ']')
                        {
                            readingState.push(std::make_pair(LIST_END, i));
                            lastDataReadState.pop();
                        }
                    else if (dataBlock[i] == '(')
                        {
                            readingState.push(std::make_pair(LIST_NAME_END, i + 1));
                        }
                    else if (dataBlock[i] == ')')
                        {
                            readingState.push(std::make_pair(LIST_SIZE_END, i));
                        }
                    else if (dataBlock[i] == ';')
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

            std::stack<std::unique_ptr<fe::feDataWriter::dataBlock>> currentBlock;
            std::stack<std::string> currentListID;
            std::stack<int> currentSize;
            while (!reverseReadingStack.empty())
                {
                    switch (reverseReadingStack.top().first)
                        {
                            case OBJ_START:
                                currentBlock.emplace(new fe::feDataWriter::dataBlock);
                                currentStorageRead.push(NONE);
                                break;
                            case OBJ_END:
                                {
                                    currentStorageRead.pop();
                                    fe::feDataWriter::dataBlock *top = currentBlock.top().release();
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
                                    char title[512] = "\0";
                                    for (unsigned int i = startIndex; i < endIndex; i++)
                                        {
                                            if (dataBlock[i] == ':')
                                                {
                                                    hasAlias = true;
                                                    startIndex = i + 1;
                                                    continue;
                                                }

                                            if (!hasAlias) 
                                                {
                                                    idName[i - startIndex] = dataBlock[i];
                                                }
                                            else
                                                {
                                                    title[i - startIndex] = dataBlock[i];
                                                }
                                        }

                                    currentBlock.top()->m_id = idName;
                                    if (hasAlias)
                                        {
                                            currentBlock.top()->m_title = title;
                                        }
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
                                    std::memcpy(data, dataBlock + startIndex, endIndex - startIndex);
                                    currentBlock.top()->readData(data);
                                }
                                break;
                            case DAT_END:
                                break;
                            case PRIMITIVE_LIST_START:
                                currentStorageRead.push(LIST);
                                break;
                            case OBJECT_LIST_START:
                                currentStorageRead.push(LIST);
                                break;
                            case LIST_END:
                                currentListID.pop();
                                currentStorageRead.pop();
                                break;
                            case LIST_NAME_START:
                                {
                                    int nameStart = reverseReadingStack.top().second + 4;
                                    reverseReadingStack.pop();
                                    int nameEnd = reverseReadingStack.top().second - 1;

                                    char name[512] = "\0";
                                    std::memcpy(name, dataBlock + nameStart, nameEnd - nameStart);
                                    currentListID.push(name);

                                    // Get the list size
                                    int sizeStart = reverseReadingStack.top().second;
                                    reverseReadingStack.pop();
                                    int sizeEnd = reverseReadingStack.top().second;
                                    char size[512] = "\0";
                                    std::memcpy(size, dataBlock + sizeStart, sizeEnd - sizeStart);
                                    currentSize.push(std::stoi(size));
                                }
                                break;
                            case LIST_ITEM_START:
                                {
                                    int itemStart = reverseReadingStack.top().second;
                                    reverseReadingStack.pop();
                                    int itemEnd = reverseReadingStack.top().second;

                                    char data[512] = "\0";
                                    std::memcpy(data, dataBlock + itemStart, itemEnd - itemStart);
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

fe::feDataWriter::feDataWriter() : m_baseDataBlock(new dataBlock("serialized_items")), m_firstRead(false)
    {
        m_currentBlock.push(m_baseDataBlock.get());
    }

fe::feDataWriter::~feDataWriter()
    {
        clearData();
    }

void fe::feDataWriter::setTitle(const std::string &title)
    {
        if (m_firstRead)
            {
                auto it = std::find_if(m_wantedData->begin(), m_wantedData->end(), [title](const std::unique_ptr<dataBlock> &data) { return data->m_title == title; });
                m_currentBlock.push(it->release());
                m_wantedData->erase(it);
                m_firstRead = false;
            }
        else 
            {
                m_currentBlock.top()->m_title = title;
            }
    }

void fe::feDataWriter::startObject(const std::string &id)
    {
        std::unique_ptr<dataBlock> *currentBlock = nullptr;
        FE_ASSERT(!m_currentBlock.empty(), "Current block is empty. Should never happen if fe::feDataWriter::feDataWriter [constructor] is called");
        m_currentBlock.top()->m_mappedObjectData[id].reset(new dataBlock(id));
        currentBlock = &m_currentBlock.top()->m_mappedObjectData[id];
        m_currentBlock.push(currentBlock->get());
    }

void fe::feDataWriter::startObjectList(const std::string &id)
    {
        std::unique_ptr<dataBlock> *currentBlock = nullptr;
        FE_ASSERT(!m_currentBlock.empty(), "Current block is empty. Should never happen if fe::feDataWriter::feDataWriter [constructor] is called");
        m_currentBlock.top()->m_mappedListObjectData[id].emplace_back(new dataBlock(id));
        currentBlock = &m_currentBlock.top()->m_mappedListObjectData[id].back();
        m_currentBlock.push(currentBlock->get());
    }

void fe::feDataWriter::startObjectRead(const std::string &id)
    {
        if (m_currentBlock.top()->m_mappedObjectData.find(id) != m_currentBlock.top()->m_mappedObjectData.end())
            {
                m_currentBlock.push(m_currentBlock.top()->m_mappedObjectData[id].get());
            }
    }

void fe::feDataWriter::startObjectListRead(const std::string &id)
    {
        if (m_currentBlock.top()->m_mappedListObjectData.find(id) != m_currentBlock.top()->m_mappedListObjectData.end())
            {
                fe::feDataWriter::dataBlock *currentBlock = m_currentBlock.top();
                m_wantedData = &currentBlock->m_mappedListObjectData[id];
                m_firstRead = true;
            }
    }

void fe::feDataWriter::endObject()
    {
        m_currentBlock.pop();
    }

void fe::feDataWriter::write(const std::string &id, const std::string &value)
    {
        m_currentBlock.top()->m_mappedData[id] = value;
    }

void fe::feDataWriter::writeList(const std::string &id, const std::string &value)
    {
        m_currentBlock.top()->m_mappedListPrimitiveData[id].emplace_back(value);
    }

std::string fe::feDataWriter::read(const std::string &id)
    {
        if (m_firstRead)
            {
                // no title has been set so we take the last value of the vector
                m_currentBlock.push(m_wantedData->back().release());
                m_wantedData->erase(m_wantedData->end() - 1);
                m_firstRead = false;
            }
        return m_currentBlock.top()->m_mappedData[id];
    }

std::string fe::feDataWriter::readList(const std::string &id)
    {
        if (m_firstRead)
            {
                // no title has been set so we take the last value of the vector
                m_currentBlock.push(m_wantedData->back().release());
                m_wantedData->erase(m_wantedData->end() - 1);
                m_firstRead = false;
            }
        std::string lastData = m_currentBlock.top()->m_mappedListPrimitiveData[id].back();
        m_currentBlock.top()->m_mappedListPrimitiveData[id].erase(m_currentBlock.top()->m_mappedListPrimitiveData[id].end() - 1);
        return lastData;
    }

void fe::feDataWriter::outData(std::ostream &out)
    {
        m_baseDataBlock->outData(out);
    }

void fe::feDataWriter::readData(std::istream &in)
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

bool fe::feDataWriter::listHasItems(const std::string &list)
    {
        if (m_currentBlock.empty()) return false;
        if (m_currentBlock.top()->m_mappedListObjectData.find(list) != m_currentBlock.top()->m_mappedListObjectData.end())
            {
                return !m_currentBlock.top()->m_mappedListObjectData[list].empty();
            }
        else if (m_currentBlock.top()->m_mappedListPrimitiveData.find(list) != m_currentBlock.top()->m_mappedListPrimitiveData.end())
            {
                return !m_currentBlock.top()->m_mappedListPrimitiveData[list].empty();
            }
        return false;
    }

void fe::feDataWriter::clearData()
    {
        m_baseDataBlock->m_mappedData.clear();
        m_baseDataBlock->m_mappedListObjectData.clear();
        m_baseDataBlock->m_mappedListPrimitiveData.clear();
        m_baseDataBlock->m_mappedObjectData.clear();

        m_baseDataBlock->m_size = 0;
    }
