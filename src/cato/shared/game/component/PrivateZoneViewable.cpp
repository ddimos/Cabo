#include "shared/game/component/PrivateZoneViewable.hpp"

#include "core/Assert.hpp"

namespace cn::shared::game::component
{

PrivateZoneViewable::PrivateZoneViewable(object::Object& _parent)
    : Component(_parent)
{
}

bool PrivateZoneViewable::isHidden() const
{
    return m_privateZone != nullptr;
}

bool PrivateZoneViewable::isHiddenInZone(object::PrivateZone& _privateZone) const
{
    return m_privateZone && m_privateZone->getId() == _privateZone.getId();
}

bool PrivateZoneViewable::isHiddenInZoneOfPlayer(PlayerId _playerId) const
{
    return m_privateZone && m_privateZone->getOwnerId() == _playerId;
}

void PrivateZoneViewable::show(object::PrivateZone& _privateZone)
{
    CN_ASSERT(m_privateZone != nullptr);
    CN_ASSERT(m_privateZone->getId() == _privateZone.getId());
    m_privateZone = nullptr;
}

void PrivateZoneViewable::hide(object::PrivateZone& _privateZone)
{
    CN_ASSERT(m_privateZone == nullptr);
    m_privateZone = &_privateZone;
}

} // namespace cn::shared::game::component
