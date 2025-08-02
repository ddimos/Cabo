#pragma once

#include "core/Context.hpp"

#include "shared/game/Anchor.hpp"

#include <SFML/Graphics/CircleShape.hpp>

namespace cn::client::game
{

class Anchor final : public shared::game::Anchor
{
public:
    Anchor(const core::Context& _context, shared::board::Anchor& _boardAnchor);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::CircleShape m_shape;
};

} // namespace cn::client::game
