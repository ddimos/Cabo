#include "core/save/Save.hpp"
#include "core/Assert.hpp"

#include <fstream>

namespace cn::core::save
{

bool Save::loadFromFile(const std::string& _filename)
{
    m_filename = _filename;

    std::ifstream file(_filename);
    CN_ASSERT(file.is_open());
    
    std::string line;
    while (std::getline(file, line))
    {
        size_t pos = line.find('|');
        m_key = line.substr(0, pos);
        m_value = line.substr(pos + 1);
    }
    return true;
}

void Save::write()
{
    std::ofstream file(m_filename);
    CN_ASSERT(file.is_open());

    file << m_key << "|" << m_value;
}

} // namespace cn::core::save
