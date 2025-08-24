#pragma once

#include "core/object/Object.hpp"

#include <SFML/Graphics/Sprite.hpp>

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

private:
    void onDraw(sf::RenderWindow& _window) override {}

    sf::Sprite m_sprite;
};

} // namespace cn::client::game
