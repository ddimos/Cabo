#pragma once

#include "core/object/Object.hpp"
#include "menu/Types.hpp"

#include <utility>

namespace cn::game
{

class Card final : public core::object::Object 
{
public:
    enum class Rank
    {
        Ace,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        _10,
        Jack,
        Queen,
        King,
        // TODO Joker
    };

    enum class Suit
    {
        Clubs,      // (♣)
        Hearts,     // (♥)
        Spades,     // (♠)
        Diamonds    // (♦)
    }; 

    enum class State
    {
        InDeck,
        OnTable,
        OnHand,
        InDiscard
    };

    static std::pair<Rank, Suit> getCardFromIndex(unsigned _index);

    Card(Rank _rank, Suit _suit, menu::item::SimpleImagePtr _image);

    void deal(/*player id*/ /*position*/);
    void discard();

    void setPosition(sf::Vector2f _position);

private:
    void onActivate() override;
    void onDeactivate() override;

    Rank m_rank;
    Suit m_suit;

    State m_state = State::InDeck;

    menu::item::SimpleImagePtr m_image;
};

} // namespace cn::game
