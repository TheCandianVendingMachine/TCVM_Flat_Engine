#include "fe/utility/splitString.hpp"

std::vector<std::string> fe::splitString(const char *str, char split)
    {
        std::vector<std::string> out;
        fe::splitString(str, split, std::move(out));
        return out;
    }

void fe::splitString(const char *str, char split, std::vector<std::string> &&out)
    {
        std::string buffer = "";
        for (unsigned int i = 0; i < std::strlen(str); i++)
            {
                if (str[i] == split)
                    {
                        out.push_back(buffer);
                        buffer = "";
                    }
                else
                    {
                        buffer += str[i];
                    }
            }

        if (!buffer.empty())
            {
                out.push_back(buffer);
            }
    }
