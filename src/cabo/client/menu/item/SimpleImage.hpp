#pragma once

#include "client/menu/Item.hpp"
#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::menu::item
{

class SimpleImage : public Item
{
public:
    SimpleImage(Position _position, const sf::Texture& _texture, sf::Color _color, float _scale);
    SimpleImage(Position _position, const sf::Texture& _texture, sf::IntRect _rectangle);
    SimpleImage(Position _position, const sf::Texture& _texture);

    void setTextureRect(sf::IntRect _rectangle);
    void setRotation(float _angleDeg);

private:
    void onDraw(sf::RenderWindow& _window) override;
    void onPositionUpdated() override;

    sf::Sprite m_sprite;
};
} // namespace cn::client::menu::item
