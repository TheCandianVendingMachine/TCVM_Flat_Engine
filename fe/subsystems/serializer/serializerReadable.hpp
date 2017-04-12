// serializerReadable.hpp
// exports the data to readable variables
#pragma once
#include <string>
#include <ostream>

class serializerReadable
    {
        protected:
            std::string m_data;
            size_t m_currentDataIndex;

            template<typename T>
            T convertValue(const std::string &in, T val) { return T(); }
            template<>
            int convertValue(const std::string &in, int val);

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
                }

    };

#define SERIALIZE_READABLE(...) \
void serialize(serializerReadable &serial) { serial.serialize(__VA_ARGS__); }\
void deserialize(serializerReadable &serial) { serial.deserialize(__VA_ARGS__); }

template<>
inline int serializerReadable::convertValue(const std::string &in, int val)
    {
        return std::stoi(in);
    }

template<>
inline float serializerReadable::convertValue(const std::string &in, float val)
    {
        return std::stof(in);
    }

template<>
inline double serializerReadable::convertValue(const std::string &in, double val)
    {
        return std::stod(in);
    }

template<typename T>
void serializerReadable::serialize(T &&data)
    {
        m_data += std::to_string(data) + "|";
    }

template<typename T, typename ...Args>
void serializerReadable::serialize(T &&data, Args &&...args)
    {
        serialize(data);
        serialize(std::forward<Args>(args)...);
        m_currentDataIndex = 0;
    }

template<typename T>
void serializerReadable::deserialize(T &newValue)
    {
        std::string data;
        int length = 0;
        for (int i = 0; i < m_data.length(); i++)
            {
                if (m_data[i] == '|')
                    {
                        length = i + 1;
                        break;
                    }
                data += m_data[i];
            }
        m_data.erase(m_data.begin(), m_data.begin() + length);

        newValue = convertValue(data, newValue);
    }

template<typename T, typename ...Args>
void serializerReadable::deserialize(T &data, Args &&...args)
    {
        deserialize(data);
        deserialize(std::forward<Args>(args)...);
        m_currentDataIndex = 0;
    }