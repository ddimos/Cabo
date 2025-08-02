#pragma once

#include "shared/game/PrivateZone.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace cn::client::game
{

class PrivateZone final : public shared::game::PrivateZone
{
public:
    PrivateZone(shared::board::PrivateZone& _boardPrivateZone);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::RectangleShape m_shape;
};

} // namespace cn::client::game
