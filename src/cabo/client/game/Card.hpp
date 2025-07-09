#pragma once

#include "core/object/Object.hpp"
#include "core/Context.hpp"

#include "shared/game/Card.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::game
{

class Card final : public shared::game::Card
{
public:
    Card(const core::Context& _context, shared::board::Card& _boardCard);
    
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

private:
    sf::Sprite m_sprite;
};

} // namespace cn::client::game
