#pragma once
#include "shared/game/component/Component.hpp"

namespace cn::shared::game::controller
{
class Attachable;
}

namespace cn::shared::game::component
{

class Attachable : public Component
{
public:
    Attachable(object::Object& _parent);

    bool isAttached() const { return m_isAttached; }
    int getRadius() const;

private:
    friend class controller::Attachable;

    void attach();
    void detach();

    bool m_isAttached = false;
};

} // namespace cn::shared::game::component
