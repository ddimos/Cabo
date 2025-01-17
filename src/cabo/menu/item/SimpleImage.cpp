#include "menu/item/SimpleImage.hpp"

namespace cn::menu::item
{

SimpleImage::SimpleImage(Position _position, const sf::Texture& _texture, sf::Color _color, float _scale)
{
    m_transformable.setPosition(_position);

    m_sprite.setTexture(_texture);
    m_sprite.setScale(_scale, _scale);
    m_sprite.setColor(_color);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(m_sprite.getGlobalBounds().getSize()));
}

SimpleImage::SimpleImage(Position _position, const sf::Texture& _texture, sf::IntRect _rectangle)
{
    m_transformable.setPosition(_position);

    m_sprite.setTexture(_texture);
    m_sprite.setTextureRect(_rectangle);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(m_sprite.getGlobalBounds().getSize()));
}

SimpleImage::SimpleImage(Position _position, const sf::Texture& _texture)
{
    m_transformable.setPosition(_position);

    m_sprite.setTexture(_texture);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(m_sprite.getGlobalBounds().getSize()));
}

void SimpleImage::setPosition(Position _position)
{
    m_transformable.setPosition(_position);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(m_sprite.getGlobalBounds().getSize()));
}

void SimpleImage::setRotation(float _angleDeg)
{
    m_sprite.setRotation(_angleDeg);
}

void SimpleImage::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::menu::item

