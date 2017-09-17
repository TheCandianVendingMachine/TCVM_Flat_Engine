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

        for (auto &memberDat : m_childDataBlocks)
            {
                memberDat.second->outData(out, preDataTextApp);
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

void fe::serializerID::interpretData(const char *block)
    {
        enum readState
            {
                OBJ_START,
                OBJ_END,
                OBJ_NAME_START,
                OBJ_NAME_END,
                DAT_START,
                DAT_END
            };
        std::stack<std::pair<readState, unsigned int>> readingState;

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
                else if (block[i] == ';')
                    {
                        readingState.push(std::make_pair(DAT_END, i));
                    }
            }

        std::stack<std::pair<readState, unsigned int>> reverseReadingStack;
        while (!readingState.empty())
            {
                reverseReadingStack.push(readingState.top());
                readingState.pop();
            }

        std::stack<std::unique_ptr<dataBlock>> currentBlock;
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
                                        currentBlock.top()->m_childDataBlocks[top->m_id].reset(top);
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
