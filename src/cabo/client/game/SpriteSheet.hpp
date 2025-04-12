#pragma once

#include "shared/game/Card.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace cn::client::game::spriteSheet
{

enum class Hover
{
    Yes,
    No
};

sf::IntRect getCardTextureRect(shared::game::Card::Rank _rank, shared::game::Card::Suit _suit);
sf::IntRect getBlurredCardTextureRect();
sf::IntRect getCardBackTextureRect(Hover _hover = Hover::No);
sf::IntRect getDiscardTextureRect(Hover _hover = Hover::No);

sf::IntRect getMatchButtonTextureRect(Hover _hover = Hover::No);
sf::IntRect getTakeButtonTextureRect(Hover _hover = Hover::No);
sf::IntRect getActionButtonTextureRect(Hover _hover = Hover::No);
sf::IntRect getDiscardButtonTextureRect(Hover _hover = Hover::No);

sf::IntRect getFinishButtonTextureRect(Hover _hover = Hover::No);

sf::IntRect getYesButtonTextureRect(Hover _hover = Hover::No);
sf::IntRect getNoButtonTextureRect(Hover _hover = Hover::No);

} // namespace cn::client::game::spriteSheet
