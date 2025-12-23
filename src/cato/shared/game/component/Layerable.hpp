#pragma once
#include "shared/game/component/Component.hpp"

namespace cn::shared::game::component
{

class Layerable : public Component
{
public:
    Layerable(object::Object& _parent);

    unsigned getIndex() const { return m_index; }
    void setIndex(unsigned _index) { m_index = _index; }

private:
    unsigned m_index = 0;
};

} // namespace cn::shared::game::component
