#pragma once
#include <fstream>

namespace utils
{
    template<typename T>
    void Read(std::ifstream& file, T& var)
    {
        file.read(reinterpret_cast<char*>(&var), sizeof(var));
    }
}
