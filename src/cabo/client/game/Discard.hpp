#pragma once

#include "shared/game/object/Discard.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace cn::client::game
{

class Discard final : public shared::game::object::Discard
{
public:
    Discard(shared::game::object::Id& _id);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::RectangleShape m_shape;
};

} // namespace cn::client::game
