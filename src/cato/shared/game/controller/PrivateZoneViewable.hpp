#pragma once

#include "shared/game/controller/Controller.hpp"
#include "shared/game/component/PrivateZoneViewable.hpp"
#include "shared/game/object/PrivateZone.hpp"

#include <functional>
#include <vector>

namespace cn::shared::game::controller
{

class PrivateZoneViewable : public Controller<component::PrivateZoneViewable>
{
public:
    using OnZoneChanged = std::function<void(component::PrivateZoneViewable&)>;
    PrivateZoneViewable(OnZoneChanged _onZoneChanged);

    void addPrivateZone(object::PrivateZone& _zone);
    void update();

private:
    OnZoneChanged m_onZoneChanged{};
    std::vector<object::PrivateZone*> m_privateZones;
};

} // namespace cn::shared::game::controller
