#pragma once

#include "shared/game/object/PrivateZone.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace cn::client::game
{

class PrivateZone final : public shared::game::object::PrivateZone
{
public:
    PrivateZone(shared::game::object::Id _id, PlayerId _ownerId);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::RectangleShape m_shape;
};

} // namespace cn::client::game
