#include "shared/net/Factory.hpp"
#include "shared/net/FactoryInitializer.hpp"

#include "core/Assert.hpp"

namespace cn::net
{

Factory::Factory()
{
    FactoryInitializer initializer(*this);
}

void Factory::add(core::event::EventId _id, SerializerCallback _serialize, DeserializerCallback _deserialize, CreationCallback _create)
{
    [[maybe_unused]] auto [it, emplaced] = m_callbacks.emplace(_id, Slot{ .serialize = _serialize, .deserialize = _deserialize, .create = _create });
    CN_ASSERT(emplaced);
}

const Factory::Slot& Factory::get(core::event::EventId _id) const
{
    return m_callbacks.at(_id);
}

} // namespace cn::net
