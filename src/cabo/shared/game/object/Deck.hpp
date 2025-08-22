#pragma once

#include "shared/game/object/Object.hpp"
#include "core/Random.hpp"

#include <functional>
#include <vector>

namespace cn::shared::game::object
{

class Card;

class Deck : public Object
{
public:
    Deck(Id _id, unsigned _seed);

    void setCards(const std::vector<Card*>& _cards);
    void shuffle();

    Card* getNextCard();

    void visit(std::function<void(const Card&)>) const;

private:
    std::vector<Card*> m_cards;
    core::Random m_randomizer;
};

} // namespace cn::shared::game::object
