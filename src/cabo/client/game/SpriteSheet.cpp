#include "client/game/SpriteSheet.hpp"

namespace
{

inline constexpr short unsigned CardWidth = 70;
inline constexpr short unsigned CardHeight = 100;

inline constexpr short unsigned FinishButtonWidth = 66;
inline constexpr short unsigned FinishButtonHeight = 20;

} // namespace

namespace cn::client::game::spriteSheet
{

sf::IntRect getCardTextureRect(shared::game::Rank _rank, shared::game::Suit _suit)
{
    int rankNum = static_cast<int>(_rank) + 1;
    int suitNum = static_cast<int>(_suit);
    return sf::IntRect{ CardWidth * rankNum , CardHeight * suitNum, CardWidth, CardHeight };
}

sf::IntRect getBlurredCardTextureRect()
{
    // TODO real blurred card
    return sf::IntRect{ 0, 2 * CardHeight, CardWidth, CardHeight };
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

sf::IntRect getFinishButtonTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ 0,                  0, FinishButtonWidth, FinishButtonHeight }
        : sf::IntRect{ 0, FinishButtonHeight, FinishButtonWidth, FinishButtonHeight };
}

sf::IntRect getCaboButtonTextureRect(Hover _hover)
{
    return getFinishButtonTextureRect(_hover);
}

} // namespace cn::client::game::spriteSheet