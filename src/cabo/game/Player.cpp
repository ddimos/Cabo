#include "game/Player.hpp"
#include "core/Types.hpp"

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
    m_nameText.setPosition(_spawnPoint.pos);
    // m_nameText.setRotation(_spawnPoint.angleDeg + 90.f);
}

void Player::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_nameText);
}

} // namespace cn::game