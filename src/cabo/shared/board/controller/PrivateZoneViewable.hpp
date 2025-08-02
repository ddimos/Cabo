#pragma once

#include "shared/board/controller/Controller.hpp"
#include "shared/board/component/PrivateZoneViewable.hpp"
#include "shared/board/PrivateZone.hpp"

#include <functional>
#include <vector>

namespace cn::shared::board::controller
{

class PrivateZoneViewable : public Controller<component::PrivateZoneViewable>
{
public:
    using OnZoneChanged = std::function<void(component::PrivateZoneViewable&)>;
    PrivateZoneViewable(OnZoneChanged _onZoneChanged);

    void addPrivateZone(PrivateZone& _zone);
    void update();

private:
    OnZoneChanged m_onZoneChanged{};
    std::vector<PrivateZone*> m_privateZones;
};

} // namespace cn::shared::board::controller
