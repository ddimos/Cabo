#pragma once

#include "core/object/Object.hpp"
#include "game/Card.hpp"
#include "game/PlayerCards.hpp"
#include "game/Types.hpp"

#include <SFML/Graphics/Text.hpp>

namespace cn::core
{
    struct Context;
}

namespace cn::game
{

class Player final : public core::object::Object
{
public:
    Player(const core::Context& _context);

    void setSpawnPoint(PlayerSpawnPoint _spawnPoint);

    void deal(const std::vector<Card>& _cards);

private:
    void onDraw(sf::RenderWindow& _window) override;

    sf::Text m_nameText;

    PlayerCards m_cards;
    PlayerSpawnPoint m_spawnPoint;
};

} // namespace cn::game
