#include "game/SpriteSheet.hpp"

namespace
{

inline constexpr short unsigned CardWidth = 70;
inline constexpr short unsigned CardHeight = 100;

} // namespace

namespace cn::game::spriteSheet
{

sf::IntRect getCardTextureRect(Card::Rank _rank, Card::Suit _suit)
{
    int rankNum = static_cast<int>(_rank) + 1;
    int suitNum = static_cast<int>(_suit);
    return sf::IntRect{ CardWidth * rankNum , CardHeight * suitNum, CardWidth, CardHeight };
}

sf::IntRect getCardBackTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ 0,          0, CardWidth, CardHeight }
        : sf::IntRect{ 0, CardHeight, CardWidth, CardHeight };
}

sf::IntRect getDiscardTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ 0, 2 * CardHeight, CardWidth, CardHeight }
        : sf::IntRect{ 0, 3 * CardHeight, CardWidth, CardHeight };
}

} // namespace cn::game::spriteSheet