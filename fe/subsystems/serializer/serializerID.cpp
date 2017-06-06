#include "serializerID.hpp"
#include "../../debug/logger.hpp"
#include <string.h>

void fe::serializerID::dataBlock::outData(std::ostream &out)
    {
        out << m_id << "{\n";
        for (auto &dat : m_mappedData)
            {
                out << dat.first << ":" << dat.second << ";\n";
            }
        out << "}\n";
    }

void fe::serializerID::dataBlock::readData(const char *block)
    {
        enum readState
            {
                ID,
                DATA,
                END
            };
        readState curState = readState::ID;

        char line[1024] = "\0";
        unsigned int index = 0;
        bool readBlock = true;

        while (readBlock)
            {
                bool foundEOL = false;
                while (!foundEOL)
                    {
                        if (block[index] != ';' && block[index] != '{' && block[index] != '}')
                            {
                                char curChar[2] = "\0";
                                curChar[0] = block[index];
                                std::strcat(line, curChar);
                            }
                        else
                            {
                                foundEOL = true;
                            }

                        if (index++ > std::strlen(block))
                            {
                                return;
                            }
                    }

                switch (curState)
                    {
                        case ID:
                            {                
                                m_id = line;
                                curState = readState::DATA;
                                line[0] = '\0';
                            }
                            break;
                        case DATA:
                            {
                                char datID[1024] = "\0";
                                std::string datStr;
                                bool findingID = true;
                                for (int i = 0; i < std::strlen(line); i++)
                                    {
                                        if (line[i] == '}')
                                            {
                                                curState = readState::END;
                                                break;
                                            }
                                        else if (line[i] == ':')
                                            {
                                                findingID = false;
                                            }
                                        else if (findingID)
                                            {
                                                char curChar[2] = "\0";
                                                curChar[0] = line[i];
                                                std::strcat(datID, curChar);
                                            }
                                        else
                                            {
                                                datStr += line[i];
                                            }
                                    }
                                line[0] = '\0';
                                m_mappedData[datID] = datStr;
                            }
                            break;
                        default:
                            break;
                    }
            }
    }

void fe::serializerID::clearData()
    {
        m_data.clear();
    }

void fe::serializerID::readData(std::istream &in)
    {
        m_data.clear();

        char datBlock[1024] = "\0";
        std::string line;
        while (std::getline(in, line))
            {
                std::strcat(datBlock, line.c_str());
                if (line == "}")
                    {
                        dataBlock newData;
                        newData.readData(datBlock);
                        datBlock[0] = '\0';

                        m_data.push_back(newData);
                    }
            }
    }
