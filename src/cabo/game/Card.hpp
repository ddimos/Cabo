#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::game
{

// TODO ? Transformable
class Card
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

    Card(Rank _rank, Suit _suit, const sf::Texture& _texture);

    void draw(sf::RenderWindow& _window);

    void deal(/*player id*/ /*position*/);
    void discard();

    void setPosition(sf::Vector2f _position);
    void setRotation(float _angleDeg);

private:
    Rank m_rank;
    Suit m_suit;

    State m_state = State::InDeck;

    sf::Sprite m_sprite;
};

} // namespace cn::game
