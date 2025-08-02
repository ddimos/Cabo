#pragma once

#include "shared/game/Object.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace cn::client::game
{

class PrivateZone final : public shared::game::Object
{
public:
    PrivateZone(shared::board::Object& _boardPrivateZone);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::RectangleShape m_shape;
};

} // namespace cn::client::game
