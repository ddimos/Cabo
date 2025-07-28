#pragma once
#include "shared/board/component/Component.hpp"

namespace cn::shared::board::controller
{
class Grabbable;
}

namespace cn::shared::board::component
{

class Grabbable : public Component
{
public:
    Grabbable(Object& _parent);

    bool isGrabbed() const { return m_isGrabbed; }
    
private:
    friend class controller::Grabbable;
    
    void grab();
    void release();

    bool m_isGrabbed = false;
};

} // namespace cn::shared::board::component
