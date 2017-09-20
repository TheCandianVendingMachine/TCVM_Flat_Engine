#include "serializerID.hpp"
//#include "../../debug/logger.hpp"
#include <string.h>
#include <stack>

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
                out << preDataText << "\tils " << listDat.first << ":[\n";
                for (auto &item : listDat.second)
                    {
                        out << preDataTextApp << "\titm " << item << ";\n";
                    }
                out << preDataText << "\t]\n";
            }

        for (auto &memberDat : m_childDataBlocks)
            {
                memberDat.second->outData(out, preDataTextApp);
            }

        std::strcat(preDataTextApp, "\t");
        for (auto &listDat : m_mappedListObjectData)
            {
                out << preDataText << "\tols " << listDat.first << ":[\n";
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
        if (m_read) return false;
        if (m_id == dataId) return true;
        for (auto &childObject : m_childDataBlocks)
            {
                if (childObject.second->hasData(dataId)) return true;
            }

        return false;
    }

fe::serializerID::dataBlock *fe::serializerID::getDataBlock(dataBlock *initial, const char *id)
    {
        if (initial->m_id == id && !initial->m_read) return initial;
        dataBlock *ret = nullptr;
        for (auto &child : initial->m_childDataBlocks)
            {
                ret = getDataBlock(child.second.get(), id);
                if (ret) break;
            }
        return ret;
    }

fe::serializerID::dataBlock *fe::serializerID::getDataBlock(dataBlock *initial)
    {
        if (!initial->m_read) return initial;
        dataBlock *ret = nullptr;
        for (auto &child : initial->m_childDataBlocks)
            {
                ret = getDataBlock(child.second.get());
                if (ret) break;
            }
        return ret;
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
                else if (block[i] == '[')
                    {
                        readingState.push(std::make_pair(LIST_NAME_END, i));
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
        while (!reverseReadingStack.empty())
            {
                switch (reverseReadingStack.top().first)
                    {
                        case OBJ_START:
                            currentBlock.emplace(new dataBlock);
                            break;
                        case OBJ_END:
                            {
                                dataBlock *top = currentBlock.top().release();
                                currentBlock.pop();
                                if (!currentBlock.empty())
                                    {
                                        switch(currentStorageRead.top())
                                            {
                                                case NONE:
                                                    currentBlock.top()->m_childDataBlocks.emplace_back(top->m_id, top);
                                                    break;
                                                case LIST:
                                                    currentBlock.top()->m_mappedListObjectData[currentListID.top()].emplace_back(top);
                                                    break;
                                                default:
                                                    break;
                                            }
                                        top->m_child = true;
                                    }
                                else
                                    {
                                        m_data.emplace_back(top);
                                    }
                            }
                            break;
                        case OBJ_NAME_START:
                            {
                                unsigned int startIndex = reverseReadingStack.top().second;
                                reverseReadingStack.pop();
                                unsigned int endIndex = reverseReadingStack.top().second;

                                char data[512] = "\0";
                                std::memcpy(data, block + startIndex, endIndex - startIndex);
                                currentBlock.top()->m_id = data;
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
                            currentStorageRead.pop();
                            break;
                        case LIST_NAME_START:
                            {
                                int nameStart = reverseReadingStack.top().second + 4;
                                reverseReadingStack.pop();
                                int nameEnd = reverseReadingStack.top().second - 1;

                                char name[512] = "\0";
                                std::memcpy(name, block + nameStart, nameEnd - nameStart);
                                currentListID.push(name);
                            }
                            break;
                        case LIST_NAME_END:
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

void fe::serializerID::clearData()
    {
        m_data.clear();
    }

void fe::serializerID::readData(std::istream &in)
    {
        m_data.clear();

        std::string datBlock;
        std::string line;
        while (std::getline(in, line))
            {
                datBlock += line;
            }

        interpretData(datBlock.c_str());
    }

bool fe::serializerID::objectExists(const char *id)
    {
        for (auto &object : m_data)
            {
                if (object->hasData(id)) return true;
            }
        return false;
    }
