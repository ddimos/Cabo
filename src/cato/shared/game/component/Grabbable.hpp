#pragma once
#include "shared/game/component/Component.hpp"

namespace cn::shared::game::controller
{
class Grabbable;
}

namespace cn::shared::game::component
{

class Grabbable : public Component
{
public:
    Grabbable(object::Object& _parent);

    bool isGrabbed() const { return m_isGrabbed; }
    
private:
    friend class controller::Grabbable;
    
    void grab();
    void release();

    bool m_isGrabbed = false;
};

} // namespace cn::shared::game::component
