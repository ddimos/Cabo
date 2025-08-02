#pragma once
#include "shared/board/component/Component.hpp"
#include "shared/board/PrivateZone.hpp"

namespace cn::shared::board::controller
{
class PrivateZoneViewable;
}

namespace cn::shared::board::component
{

class PrivateZoneViewable : public Component
{
public:
    PrivateZoneViewable(Object& _parent);

    bool isHidden() const;
    bool isHiddenInZone(PrivateZone& _privateZone) const;
    bool isHiddenInZoneOfPlayer(PlayerId _playerId) const;

    PrivateZone* getPrivateZone() const { return m_privateZone; }

private:
    friend class controller::PrivateZoneViewable;

    void show(PrivateZone& _privateZone);
    void hide(PrivateZone& _privateZone);

    PrivateZone* m_privateZone = nullptr;
};

} // namespace cn::shared::board::component
