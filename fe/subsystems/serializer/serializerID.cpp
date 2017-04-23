#include "serializerID.hpp"

void fe::serializerID::dataBlock::outData(std::ostream &out)
    {
        out << m_id << "{\n";
        for (auto &dat : m_mappedData)
            {
                out << dat.first << ":" << dat.second << ";\n";
            }
        out << "}\n";
    }

void fe::serializerID::dataBlock::readData(const std::string &block)
    {
        enum readState
            {
                ID,
                DATA,
                END
            };
        readState curState = readState::ID;

        std::string line;
        unsigned int index = 0;
        bool readBlock = true;

        while (readBlock)
            {
                bool foundEOL = false;
                while (!foundEOL)
                    {
                        if (block[index] != ';' && block[index] != '{' && block[index] != '}')
                            {
                                line += block[index];
                            }
                        else
                            {
                                foundEOL = true;
                            }

                        if (index++ >= block.size())
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
                                line.clear();
                            }
                            break;
                        case DATA:
                            {
                                std::string datID;
                                std::string datStr;
                                bool findingID = true;
                                for (int i = 0; i < line.size(); i++)
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
                                                datID += line[i];
                                            }
                                        else
                                            {
                                                datStr += line[i];
                                            }
                                    }
                                line.clear();
                                m_mappedData[datID] = datStr;
                            }
                            break;
                        default:
                            break;
                    }
            }
    }

void fe::serializerID::readData(std::istream &in)
    {
        m_data.clear();

        std::string datBlock;
        std::string line;
        while (std::getline(in, line))
            {
                datBlock += line;
                if (line == "}")
                    {
                        dataBlock newData;
                        newData.readData(datBlock);
                        datBlock.clear();

                        m_data.push_back(newData);
                    }
            }
    }
