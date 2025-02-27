#include "client/game/SpriteSheet.hpp"

namespace
{

inline constexpr short unsigned CardWidth = 70;
inline constexpr short unsigned CardHeight = 100;

inline constexpr short unsigned DecideButtonWidth = 66;
inline constexpr short unsigned DecideButtonHeight = 20;

inline constexpr short unsigned FinishButtonWidth = 66;
inline constexpr short unsigned FinishButtonHeight = 20;


} // namespace

namespace cn::client::game::spriteSheet
{

sf::IntRect getCardTextureRect(shared::game::Card::Rank _rank, shared::game::Card::Suit _suit)
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

sf::IntRect getMatchButtonTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ 0,                  0, DecideButtonWidth, DecideButtonHeight }
        : sf::IntRect{ 0, DecideButtonHeight, DecideButtonWidth, DecideButtonHeight };
}

sf::IntRect getTakeButtonTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ DecideButtonWidth,                  0, DecideButtonWidth, DecideButtonHeight }
        : sf::IntRect{ DecideButtonWidth, DecideButtonHeight, DecideButtonWidth, DecideButtonHeight };
}

sf::IntRect getActionButtonTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ 2 * DecideButtonWidth,                  0, DecideButtonWidth, DecideButtonHeight }
        : sf::IntRect{ 2 * DecideButtonWidth, DecideButtonHeight, DecideButtonWidth, DecideButtonHeight };
}

sf::IntRect getDiscardButtonTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ 3 * DecideButtonWidth,                  0, DecideButtonWidth, DecideButtonHeight }
        : sf::IntRect{ 3 * DecideButtonWidth, DecideButtonHeight, DecideButtonWidth, DecideButtonHeight };
}

sf::IntRect getFinishButtonTextureRect(Hover _hover)
{
    return _hover == Hover::No 
        ? sf::IntRect{ 0,                  0, FinishButtonWidth, FinishButtonHeight }
        : sf::IntRect{ 0, FinishButtonHeight, FinishButtonWidth, FinishButtonHeight };
}

} // namespace cn::client::game::spriteSheet