#pragma once

#include "core/Context.hpp"
#include "shared/game/object/Anchor.hpp"

#include <SFML/Graphics/CircleShape.hpp>

namespace cn::client::game
{

class Anchor final : public shared::game::object::Anchor
{
public:
    Anchor(const core::Context& _context, shared::game::object::Id _id, int _radius);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::CircleShape m_shape;
};

} // namespace cn::client::game
