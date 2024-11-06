#pragma once

#include "core/Assert.hpp"

#include <unordered_map>
#include <string>
#include <memory>

namespace cn::core
{

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    void load(Identifier _id, const std::string& _path);

    template <typename Parameter>
    void load(Identifier _id, const std::string& _path, const Parameter& _secondParam);

    Resource& get(Identifier _id);
    const Resource& get(Identifier _id) const;

private:
    void insertResource(Identifier _id, std::unique_ptr<Resource> _resource);

    std::unordered_map<Identifier, std::unique_ptr<Resource>> m_map;
};


template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier _id, const std::string& _path)
{
    std::unique_ptr<Resource> resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(_path))
    {
        CN_ASSERT(false, "Failed to load a resource");
        return;
    }
    insertResource(_id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier _id, const std::string& _path, const Parameter& _secondParam)
{
    std::unique_ptr<Resource> resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(_path, _secondParam))
    {
        CN_ASSERT(false, "Failed to load a resource");
        return;
    }
    insertResource(_id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier _id)
{
    auto resourceIt = m_map.find(_id);
    CN_ASSERT(resourceIt != m_map.end(), "The resource wasn't found");
    return *resourceIt->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier _id) const
{
    auto resourceIt = m_map.find(_id);
    CN_ASSERT(resourceIt != m_map.end(), "The resource wasn't found");
    return *resourceIt->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier _id, std::unique_ptr<Resource> _resource) 
{
    auto result = m_map.insert_or_assign(_id, std::move(_resource));
    CN_ASSERT(result.second, "The resource wasn't inserted");
}

} // namespace cn::core
