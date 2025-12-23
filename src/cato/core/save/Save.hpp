#pragma once

#include <string>

namespace cn::core::save
{

class Save
{
public:
    Save() = default;

    bool loadFromFile(const std::string& _filename);
    void write();

    const std::string& getKey() const { return m_key; }
    void setKey(const std::string& _key) { m_key = _key; }

    const std::string& getValue() const { return m_value; }
    void setValue(const std::string& _value) { m_value = _value; }

private:
    std::string m_filename;

    std::string m_key;
    std::string m_value;
};

} // namespace cn::core::save
