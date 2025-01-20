#pragma once

#include "core/object/Object.hpp"
#include "core/Random.hpp"

#include "game/object/Card.hpp"
#include "game/Types.hpp"
#include "client/menu/Types.hpp"

#include <vector>

namespace cn::game
{

class Deck final : public core::object::Object
{
public:
    Deck(client::menu::item::ButtonPtr _button, std::vector<CardPtr>&& _cards, unsigned _seed);

    void shuffle();
    void shuffleFromDiscard();

    CardPtr getNextCard();

private:
    client::menu::item::ButtonPtr m_button;
    std::vector<CardPtr> m_cards;
    core::Random m_randomizer;
};

} // namespace cn::game
