#pragma once
#include "shared/game/component/Component.hpp"

namespace cn::shared::game::controller
{
class Flippable;
}

namespace cn::shared::game::component
{

class Flippable : public Component
{
public:
    Flippable(object::Object& _parent);

    bool isFaceUp() const { return m_isFaceUp; }
    
private:
    friend class controller::Flippable;

    void turnUp();
    void turnDown();

    bool m_isFaceUp = false;
};

} // namespace cn::shared::game::component
