// serializerBinary.hpp
// Casts data into binary numbers and stores the values
#pragma once
#include <string>
#include <ostream>

class serializerBinary
    {
        protected:
            std::string m_data;
            size_t m_currentDataIndex;

        public:
            void serialize() 
                {}

            template<typename T>
            void serialize(T &&data);

            template<typename T, typename ...Args>
            void serialize(T &&data, Args &&...args);

            void deserialize() {}
            template<typename T>
            void deserialize(T &newValue);

            template<typename T, typename ...Args>
            void deserialize(T &newValue, Args &&...args);

            void outData(std::ostream &out)
                {
                    out << m_data << "\n";
                }

            void readData(std::istream &in)
                {
                    m_currentDataIndex = 0;
                    std::string line;
                    while (std::getline(in, line))
                        {
                            m_data += line;
                        }

                    int i = 0;
                }

    };

#define SERIALIZE_BINARY(...) \
void serialize(serializerBinary &serial) { serial.serialize(__VA_ARGS__); }\
void deserialize(serializerBinary &serial) { serial.deserialize(__VA_ARGS__); }

template<typename T>
void serializerBinary::serialize(T &&data)
    {
        std::uint8_t *bytes = reinterpret_cast<uint8_t*>(&data);
        for (unsigned int i = 0; i < sizeof(T) * 8; i++)
            {
                m_data += bytes[i];
            }
    }

template<typename T, typename ...Args>
void serializerBinary::serialize(T &&data, Args &&...args)
    {
        serialize(data);
        serialize(std::forward<Args>(args)...);
        m_currentDataIndex = 0;
    }

template<typename T>
void serializerBinary::deserialize(T &newValue)
    {
        std::uint8_t dataBuffer[sizeof(T) * 8];
        std::memcpy(dataBuffer, m_data.c_str() + m_currentDataIndex, sizeof(T) * 8);
        newValue = *reinterpret_cast<T*>(dataBuffer);
        m_currentDataIndex += sizeof(T) * 8;
    }

template<typename T, typename ...Args>
void serializerBinary::deserialize(T &newValue, Args &&...args)
    {
        deserialize(newValue);
        deserialize(std::forward<Args>(args)...);
    }
