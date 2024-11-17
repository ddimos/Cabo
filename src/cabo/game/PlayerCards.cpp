#include "game/PlayerCards.hpp"
#include "core/math/Math.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::game
{

PlayerCards::PlayerCards()
{
}

void PlayerCards::setSpawnPoint(PlayerSpawnPoint _spawnPoint)
{
    float offsetBetweenCards = 100;
    int i = 0;
    for (auto& card : m_cards)
    {
        sf::Vector2f localPos = sf::Vector2f(i * offsetBetweenCards - 200, 20);
        localPos = core::math::rotateVector(localPos, _spawnPoint.angleDeg + 90.f);
        card.setPosition(_spawnPoint.pos + localPos);
        card.setRotation(_spawnPoint.angleDeg + 90.f);
        ++i;
    }
}

void PlayerCards::setCards(const std::vector<Card>& _cards)
{
    m_cards = _cards;
}

void PlayerCards::draw(sf::RenderWindow& _window)
{
    for (auto& card : m_cards)
        card.draw(_window);
}

} // namespace cn::game
