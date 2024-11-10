#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace cn::menu::component
{

class Drawable
{
public:
    void setDrawPosition(const sf::Vector2f& _position);
    void setTexture(const sf::Texture& _texture);
    void setTextureRect(const sf::IntRect& _rect);

protected:
    sf::FloatRect getDrawBounds() const;
    void draw(sf::RenderWindow& _windowRef);

private:
    sf::Sprite m_sprite;
};

} // namespace cn::menu::component
