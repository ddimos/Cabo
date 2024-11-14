#pragma once

#include "core/object/Object.hpp"
#include "core/Random.hpp"

#include "game/Card.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <vector>

namespace sf
{
    class RenderWindow;
    class Texture;
}

namespace cn::game
{

class Deck final : public core::object::Object
{
public:
    Deck(const sf::Texture& _backCardTexture, short unsigned _size, unsigned _seed);

    void shuffle();
    void shuffleFromDiscard();

private:
    void onDraw(sf::RenderWindow& _window);

    std::vector<Card> m_cards;
    core::Random m_randomizer;

    sf::Sprite m_backCardSprite;
};

} // namespace cn::game
