#pragma once

#include "game/Card.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace cn::game::spriteSheet
{

enum class Hover
{
    Yes,
    No
};

sf::IntRect getCardTextureRect(Card::Rank _rank, Card::Suit _suit);
sf::IntRect getCardBackTextureRect(Hover _hover = Hover::No);
sf::IntRect getDiscardTextureRect(Hover _hover = Hover::No);

} // namespace cn::game::spriteSheet
