#pragma once

#include "core/object/Object.hpp"
#include "menu/Types.hpp"

namespace cn::game
{

class Card final : public core::object::Object 
{
public:
    enum class Rank
    {
        Ace,
        _1,
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
        Joker
    };

    enum class Suit
    {
        Clubs,      // (♣)
        Diamonds,   // (♦)
        Hearts,     // (♥)
        Spades      // (♠)
    }; 

    enum class State
    {
        InDeck,
        OnTable,
        OnHand,
        InDiscard
    };

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
