#pragma once

#include "shared/game/object/Discard.hpp"
#include "core/Context.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::game
{

class Discard final : public shared::game::object::Discard
{
public:
    Discard(const core::Context& _context, shared::game::object::Id& _id);

private:
    void onActivate() override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::Sprite m_sprite;
};

} // namespace cn::client::game
