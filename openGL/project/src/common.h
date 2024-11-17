#pragma once
#include <iostream>
#include <string>
#include <new>
#include <fstream>
#include <sstream>

class File
{
public:
    static File& GetInstance();
    std::string readFile(const std::string &fileName);

private:
    File() {};
    ~File() {};
    File(const File &file) = delete;
    const File &operator=(const File &file) = delete;
};