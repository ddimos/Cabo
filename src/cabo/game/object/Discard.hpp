#pragma once

#include "core/object/Object.hpp"
#include "game/Types.hpp"

#include "client/menu/Types.hpp"

namespace cn::game
{

class Discard final : public core::object::Object
{
public:
    Discard(client::menu::item::ButtonPtr _button);

    void discard(CardPtr _card);
    CardPtr getLast();
    
private:
    client::menu::item::ButtonPtr m_button;
    std::vector<CardPtr> m_cards;
};

} // namespace cn::game
