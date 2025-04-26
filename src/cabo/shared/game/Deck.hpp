#pragma once

#include "core/object/Object.hpp"
#include "core/Random.hpp"

#include "server/game/Card.hpp"

#include <functional>
#include <vector>

namespace cn::shared::game
{

class Deck final : public core::object::Object
{
public:
    Deck(std::vector<Card*>&& _cards, unsigned _seed);

    void shuffle();
    void shuffleFromDiscard();

    Card* getNextCard();

    void visit(std::function<void(const Card&)>) const;

private:
    std::vector<Card*> m_cards;
    core::Random m_randomizer;
};

} // namespace cn::game
