#pragma once

#include "client/menu/item/SimpleImage.hpp"
#include "shared/game/Card.hpp"

namespace cn::client::game
{

class Card final : public shared::game::Card
{
public:
    Card(Rank _rank, Suit _suit, menu::item::SimpleImage& _image);

    // TODO not sure I need all these methods
    void setPosition(sf::Vector2f _position);
    void setRotation(float _angleDeg);

private:
    void onActivate() override;
    void onDeactivate() override;

    client::menu::item::SimpleImage& m_image;
};

} // namespace cn::client::game
