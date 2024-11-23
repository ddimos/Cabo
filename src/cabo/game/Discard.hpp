#pragma once

#include "core/object/Object.hpp"
#include "game/Types.hpp"

#include "menu/Types.hpp"

namespace cn::game
{

class Discard final : public core::object::Object
{
public:
    Discard(menu::item::ButtonPtr _button);

    void discard(CardPtr _card);
    CardPtr getLast();
    
private:
    menu::item::ButtonPtr m_button;
    std::vector<CardPtr> m_cards;
};

} // namespace cn::game
