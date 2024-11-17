#pragma once

#include "core/object/Object.hpp"
#include "game/Types.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

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

private:
    void onDraw(sf::RenderWindow& _window) override;

    sf::Text m_nameText;
};

} // namespace cn::game
