#pragma once

#include "core/Assert.hpp"

#include <memory>
#include <cstdint>
#include <unordered_map>

namespace cn::core
{

class Context final
{
    using ObjectId = uint8_t;

    template <typename TObject>
    class IdHolder final
    {
    public:
        static void assign(ObjectId _id) { m_id = _id; }
        static ObjectId get() { return m_id; }

    private:
        inline static ObjectId m_id = 0;
    };

    class ObjectHolderBase
    {
    public:
        virtual ~ObjectHolderBase() = default;
    };

    template <typename TObject>
    class ObjectHolder : public ObjectHolderBase
    {
    public:
        ObjectHolder(TObject& _objectRef) : m_objectRef(_objectRef) {}
        
        TObject& get() { return m_objectRef; }
        const TObject& get() const { return m_objectRef; }

    private:
        TObject& m_objectRef;
    };

public:
    template <typename TObject>
    void insert(TObject& _objectRef)
    {
        ObjectId id = ++m_objectIdGenerator;
        CN_ASSERT(IdHolder<TObject>::get() == 0);
        IdHolder<TObject>::assign(id);
        CN_ASSERT(m_map.find(id) == m_map.end());
        [[maybe_unused]] auto [it, emplaced] = m_map.emplace(id, std::make_unique<ObjectHolder<TObject>>(_objectRef));
        CN_ASSERT(emplaced);
    }

    template <typename TObject>
    TObject& get() const
    {
        ObjectId id = IdHolder<TObject>::get();
        auto it = m_map.find(id);
        CN_ASSERT(it != m_map.end());
        return static_cast<ObjectHolder<TObject>&>(*(it->second)).get();
    }

private:
    ObjectId m_objectIdGenerator = 0;
    std::unordered_map<ObjectId, std::unique_ptr<ObjectHolderBase>> m_map;
};

} // namespace cn::core
