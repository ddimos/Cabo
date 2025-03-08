#include "client/menu/item/SimpleImage.hpp"

namespace cn::client::menu::item
{

SimpleImage::SimpleImage(Position _position, const sf::Texture& _texture, sf::Color _color, float _scale)
{
    m_transformable.setPosition(_position);

    m_sprite.setTexture(_texture);
    m_sprite.setScale(_scale, _scale);
    m_sprite.setColor(_color);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(sf::Vector2f{}));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

SimpleImage::SimpleImage(Position _position, const sf::Texture& _texture, sf::IntRect _rectangle)
{
    m_transformable.setPosition(_position);

    m_sprite.setTexture(_texture);
    m_sprite.setTextureRect(_rectangle);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(sf::Vector2f{}));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

SimpleImage::SimpleImage(Position _position, const sf::Texture& _texture)
{
    m_transformable.setPosition(_position);

    m_sprite.setTexture(_texture);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(sf::Vector2f{}));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void SimpleImage::setTextureRect(sf::IntRect _rectangle)
{
    m_sprite.setTextureRect(_rectangle);
    // should I recalculate the position?
}

void SimpleImage::setPosition(Position _position)
{
    m_transformable.setPosition(_position);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(sf::Vector2f{}));
}

void SimpleImage::setRotation(float _angleDeg)
{
    m_sprite.setRotation(_angleDeg);
}

void SimpleImage::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::menu::item

