#pragma once

#include "shared/game/object/Deck.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace cn::client::game
{

class Deck final : public shared::game::object::Deck
{
public:
    Deck(shared::game::object::Id _id, unsigned _seed);

private:
    void onActivate() override;
    void onDraw(sf::RenderWindow& _window) override;

    sf::RectangleShape m_shape;
};

} // namespace cn::client::game
