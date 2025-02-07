#pragma once

#include "core/object/Object.hpp"
#include "client/game/Types.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <vector>

namespace cn::core
{
    class Context;
}

namespace cn::client::game
{

class Table final : public core::object::Object
{

public:
    Table(const core::Context& _context);

    std::vector<PlayerSpawnPoint> generateSpawnPoints(unsigned _count, sf::Vector2f _windowHalfSize);

private:
    void onDraw(sf::RenderWindow& _window) override;

    sf::Sprite m_sprite;
    std::vector<PlayerSpawnPoint> m_spawnPoints;
};

} // namespace cn::client::game
