#pragma once

#include "core/event/Types.hpp"

#include <nsf/Types.hpp>

#include <functional>
#include <memory>
#include <unordered_map>

namespace cn::net
{

class FactoryInitializer;

class Factory
{
public:
    using SerializerCallback = std::function<void(const core::event::Event&, nsf::Buffer&)>;
    using DeserializerCallback = std::function<void(core::event::Event&, nsf::Buffer&)>;
    using CreationCallback = std::function<std::unique_ptr<core::event::Event>(nsf::PeerID _peerId)>;
    
    struct Slot
    {
        SerializerCallback serialize;
        DeserializerCallback deserialize;
        CreationCallback create;
    };

    Factory();

    const Slot& get(core::event::EventId _id) const;

private:
    friend class FactoryInitializer;
    
    void add(core::event::EventId _id, SerializerCallback _serialize, DeserializerCallback _deserialize, CreationCallback _create);

    std::unordered_map<core::event::EventId, Slot> m_callbacks;
};

} // namespace cn::net
