#pragma once

#include "game/Card.hpp"
#include "game/Types.hpp"

#include <vector>

namespace sf
{
    class RenderWindow;
} // namespace sf

namespace cn::game
{
class PlayerCards
{

public:
    PlayerCards();

    void setSpawnPoint(PlayerSpawnPoint _spawnPoint);
    void setCards(const std::vector<Card>& _cards);

    void draw(sf::RenderWindow& _window);

private:
    std::vector<Card> m_cards;
};

} // namespace cn::game
