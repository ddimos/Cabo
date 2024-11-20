#pragma once

#include "core/object/Object.hpp"
#include "game/Card.hpp"

#include "menu/Types.hpp"

namespace cn::game
{

class Discard final : public core::object::Object
{
public:
    Discard(menu::item::ButtonPtr _button);

private:
    menu::item::ButtonPtr m_button;
    std::vector<Card> m_cards;
};

} // namespace cn::game
