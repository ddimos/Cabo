#include "shared/board/component/PrivateZoneViewable.hpp"

#include "core/Assert.hpp"

namespace cn::shared::board::component
{

PrivateZoneViewable::PrivateZoneViewable(Object& _parent)
    : Component(_parent)
{
}

bool PrivateZoneViewable::isHidden() const
{
    return m_privateZone != nullptr;
}

bool PrivateZoneViewable::isHiddenInZone(PrivateZone& _privateZone) const
{
    return m_privateZone && m_privateZone->getId() == _privateZone.getId();
}

bool PrivateZoneViewable::isHiddenInZoneOfPlayer(PlayerId _playerId) const
{
    return m_privateZone && m_privateZone->getOwnerId() == _playerId;
}

void PrivateZoneViewable::show(PrivateZone& _privateZone)
{
    CN_ASSERT(m_privateZone != nullptr);
    CN_ASSERT(m_privateZone->getId() == _privateZone.getId());
    m_privateZone = nullptr;
}

void PrivateZoneViewable::hide(PrivateZone& _privateZone)
{
    CN_ASSERT(m_privateZone == nullptr);
    m_privateZone = &_privateZone;
}

} // namespace cn::shared::board::component
