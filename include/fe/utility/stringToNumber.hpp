// stringToNumber.hpp
// parses string and returns the number.
#pragma once
#include <string>

namespace fe
    {
        template<typename T>
        T stringToNumber(const std::string &str)
            {
                double number = 0.0;
                double decimal = 0.0;
                double *appendingNum = &number;

                int decimalSize = 10;
                for (unsigned int i = 0; i < str.size(); i++)
                    {
                        char c = str.at(i);
                        if (c == '.')
                            {
                                decimalSize = 10;
                                appendingNum = &decimal;
                            }
                        else if (c >= '0' && c <= '9')
                            {
                                (*appendingNum) = ((*appendingNum) * 10) + (c - '0');
                                decimalSize *= 10;
                            }
                    }

                decimal /= (decimalSize / 10);

                if (str.at(0) == '-')
                    {
                        number = -number;
                        decimal = -decimal;
                    }

                return static_cast<T>(number + decimal);
            }
    }
