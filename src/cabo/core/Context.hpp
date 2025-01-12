#pragma once

#include "core/Assert.hpp"

#include <memory>
#include <cstdint>
#include <unordered_map>

namespace cn::core
{

using ContextObjectId = uint8_t;

template <typename TObject> 
ContextObjectId getContextObjectId()
{
    // TODO a way to use the static assert without including the header with specializations.
    // I don't want to include "game" files to the core
    CN_ASSERT(false);
    // static_assert(false, "getContextObjectId function should be specialized");
    return 0;
}

class ContextObjectHolderBase
{
public:
    virtual ~ContextObjectHolderBase() = default;
};

template <typename TObject>
class ContextObjectHolder : public ContextObjectHolderBase
{
public:
    ContextObjectHolder(TObject& _objectRef)
        : m_objectRef(_objectRef)
    {}
    
    TObject& get() { return m_objectRef; }
    const TObject& get() const { return m_objectRef; }

private:
    TObject& m_objectRef;
};

class Context
{
public:
    template <typename TObject>
    void insert(TObject& _objectRef)
    {
        ContextObjectId id = getContextObjectId<TObject>();
        CN_ASSERT(m_map.find(id) == m_map.end());
        [[maybe_unused]] auto [it, emplaced] = m_map.emplace(id, std::make_unique<ContextObjectHolder<TObject>>(_objectRef));
        CN_ASSERT(emplaced);
    }

    template <typename TObject>
    TObject& get() const
    {
        ContextObjectId id = getContextObjectId<TObject>();
        auto it = m_map.find(id);
        CN_ASSERT(it != m_map.end());
        return static_cast<ContextObjectHolder<TObject>&>(*(it->second)).get();
    }

private:
    std::unordered_map<ContextObjectId, std::unique_ptr<ContextObjectHolderBase>> m_map;
};

} // namespace cn::core
