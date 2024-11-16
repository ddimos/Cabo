#pragma once

#include "core/object/Object.hpp"
#include "game/Card.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::core
{
    struct Context;
}

namespace cn::game
{

class Discard final : public core::object::Object
{
public:
    Discard(const core::Context& _context);

private:
    void onDraw(sf::RenderWindow& _window);

    sf::Sprite m_sprite;
    std::vector<Card> m_cards;
};

} // namespace cn::game
