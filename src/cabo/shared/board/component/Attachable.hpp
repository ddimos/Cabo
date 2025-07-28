#pragma once
#include "shared/board/component/Component.hpp"

namespace cn::shared::board::controller
{
class Attachable;
}

namespace cn::shared::board::component
{

class Attachable : public Component
{
public:
    Attachable(Object& _parent);

    bool isAttached() const { return m_isAttached; }
    int getRadius() const;

private:
    friend class controller::Attachable;

    void attach();
    void detach();

    bool m_isAttached = false;
};

} // namespace cn::shared::board::component
