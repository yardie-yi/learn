#include "common.h"


File& File::GetInstance()
{
    static File instance;
    return instance;
}

std::string File::readFile(const std::string &fileName)
{
    std::ifstream file(fileName);
    std::stringstream ss;

    std::string line;
    while(getline(file, line))
    {
        ss << line << '\n';
        std::cout << line << std::endl;
    }
    return ss.str();
}
