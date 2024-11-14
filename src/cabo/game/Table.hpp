#pragma once

#include "core/object/Object.hpp"
#include "core/Types.hpp"

#include "game/Card.hpp"
#include "game/Deck.hpp"
#include "game/Types.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

namespace cn::game
{

class Table final : public core::object::Object
{
public:
    Table(const sf::Texture& _tableTexture, DeckPtr _deck, unsigned _seed);

private:
    void onUpdate(sf::Time _dt);
    void onDraw(sf::RenderWindow& _window);

    sf::Sprite m_sprite;

    DeckPtr m_deck;
    std::vector<Card> m_discard;
};

} // namespace cn::game
