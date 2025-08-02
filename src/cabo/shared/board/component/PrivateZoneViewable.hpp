#pragma once
#include "shared/board/component/Component.hpp"

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
    bool isHiddenInZone(Object& _privateZone) const;

private:
    friend class controller::PrivateZoneViewable;

    void show(Object& _privateZone);
    void hide(Object& _privateZone);

    Object* m_privateZone = nullptr;
};

} // namespace cn::shared::board::component
