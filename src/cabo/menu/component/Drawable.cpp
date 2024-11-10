#include "menu/component/Drawable.hpp"

namespace cn::menu::component
{

void Drawable::setDrawPosition(const sf::Vector2f& _position)
{
    m_sprite.setPosition(_position);
}

void Drawable::setTexture(const sf::Texture& _texture)
{
    m_sprite.setTexture(_texture);
}

void Drawable::setTextureRect(const sf::IntRect& _rect)
{
    m_sprite.setTextureRect(_rect);
}

sf::FloatRect Drawable::getDrawBounds() const
{
    return m_sprite.getGlobalBounds();
}

void Drawable::draw(sf::RenderWindow& _windowRef)
{
    _windowRef.draw(m_sprite);
}

} // namespace cn::menu::component