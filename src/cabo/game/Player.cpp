#include "game/Player.hpp"
#include "core/Types.hpp"
// #include "core/math/Math.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::game
{

Player::Player(const core::Context& _context)
{
    m_nameText.setFont(_context.fontHolderRef.get(FontIds::Main));
    m_nameText.setString("Player");
    m_nameText.setFillColor(sf::Color::White);
    m_nameText.setCharacterSize(24);
    m_nameText.setOrigin(m_nameText.getGlobalBounds().getSize() / 2.f);
}

void Player::setSpawnPoint(PlayerSpawnPoint _spawnPoint)
{
    m_spawnPoint = _spawnPoint;
    m_nameText.setPosition(_spawnPoint.pos);
}

void Player::deal(const std::vector<Card>& _cards)
{
    m_cards.setCards(_cards);
    m_cards.setSpawnPoint(m_spawnPoint);
}

void Player::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_nameText);
    m_cards.draw(_window);
}

} // namespace cn::game