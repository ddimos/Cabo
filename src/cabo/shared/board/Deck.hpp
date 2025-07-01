#pragma once

#include "core/Random.hpp"

#include <functional>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace cn::shared::board
{

class Card;

class Deck final
{
public:
    Deck(unsigned _seed, sf::Vector2f _position);

    void setCards(std::vector<Card*>&& _cards);
    void shuffle();

    Card* getNextCard();

    void visit(std::function<void(const Card&)>) const;

private:
    std::vector<Card*> m_cards;
    core::Random m_randomizer;
    
    const sf::Vector2f m_position{};
};

} // namespace cn::shared::board
