#include "game/Player.hpp"

namespace cn::game
{

Player::Player()
{
}

void Player::deal(const std::vector<Card>& _cards)
{
    m_cards = _cards;
}

} // namespace cn::game