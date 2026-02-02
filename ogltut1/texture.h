#pragma once
#include <string> 
class Texture
{
public:
    Texture(const std::string& path);
    void Bind(unsigned int slot = 0) const; // declared

private:
    unsigned int m_ID;
};
