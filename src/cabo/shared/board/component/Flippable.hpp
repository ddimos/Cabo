#pragma once
#include "shared/board/component/Component.hpp"

namespace cn::shared::board::controller
{
class Flippable;
}

namespace cn::shared::board::component
{

class Flippable : public Component
{
public:
    Flippable(Object& _parent);

    bool isFaceUp() const { return m_isFaceUp; }
    
private:
    friend class controller::Flippable;

    void turnUp();
    void turnDown();

    bool m_isFaceUp = false;
};

} // namespace cn::shared::board::component
