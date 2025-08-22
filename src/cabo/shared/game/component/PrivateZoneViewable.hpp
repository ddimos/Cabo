#pragma once
#include "shared/game/component/Component.hpp"
#include "shared/game/object/PrivateZone.hpp"
#include "shared/player/Types.hpp"

namespace cn::shared::game::controller
{
class PrivateZoneViewable;
}

namespace cn::shared::game::component
{

class PrivateZoneViewable : public Component
{
public:
    PrivateZoneViewable(object::Object& _parent);

    bool isHidden() const;
    bool isHiddenInZone(object::PrivateZone& _privateZone) const;
    bool isHiddenInZoneOfPlayer(PlayerId _playerId) const;

    object::PrivateZone* getPrivateZone() const { return m_privateZone; }

private:
    friend class controller::PrivateZoneViewable;

    void show(object::PrivateZone& _privateZone);
    void hide(object::PrivateZone& _privateZone);

    object::PrivateZone* m_privateZone = nullptr;
};

} // namespace cn::shared::game::component
