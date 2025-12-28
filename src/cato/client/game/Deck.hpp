#pragma once

#include "shared/game/object/Deck.hpp"
#include "core/Context.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::game
{

class Deck final : public shared::game::object::Deck
{
public:
    Deck(const core::Context& _context, shared::game::object::Id _id, unsigned _seed);

private:
    void onActivate() override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::Sprite m_sprite;
};

} // namespace cn::client::game
