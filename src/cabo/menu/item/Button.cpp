#include "menu/item/Button.hpp"
#include "menu/Utils.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace cn::menu::item
{

Button::Button(const sf::Texture& _texture, const sf::IntRect& _textureRectDefault,
    const sf::IntRect& _textureRectHover, sf::Mouse::Button _button)
    : m_textureRectDefault(_textureRectDefault)
    , m_textureRectHover(_textureRectHover)
{
    m_sprite.setTexture(_texture);
    m_sprite.setTextureRect(m_textureRectDefault);
    m_clickable.setButton(_button);
    m_hoverable.setCallback(
        [this](bool _isHovered){
            if (_isHovered)
                m_sprite.setTextureRect(m_textureRectHover);
            else
                m_sprite.setTextureRect(m_textureRectDefault);
        }
    );
}

Button::Button(Position _position, const sf::Texture& _texture, const sf::IntRect& _textureRectDefault,
    const sf::IntRect& _textureRectHover, component::Clickable::Callback _onClickCallback, sf::Mouse::Button _button)
    : m_textureRectDefault(_textureRectDefault)
    , m_textureRectHover(_textureRectHover)
{
    m_transformable.setPosition(_position);

    m_sprite.setTexture(_texture);
    m_sprite.setTextureRect(m_textureRectDefault);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(m_sprite.getGlobalBounds().getSize()));

    m_clickable.setCallback(_onClickCallback);
    m_clickable.setBounds(m_sprite.getGlobalBounds());
    m_clickable.setButton(_button);

    m_hoverable.setBounds(m_sprite.getGlobalBounds());
    m_hoverable.setCallback(
        [this](bool _isHovered){
            if (_isHovered)
                m_sprite.setTextureRect(m_textureRectHover);
            else
                m_sprite.setTextureRect(m_textureRectDefault);
        }
    );
}

void Button::setPosition(Position _position)
{
    m_transformable.setPosition(_position);
    m_sprite.setPosition(m_transformable.calculateGlobalPos(m_sprite.getGlobalBounds().getSize()));
    m_clickable.setBounds(m_sprite.getGlobalBounds());
    m_hoverable.setBounds(m_sprite.getGlobalBounds());
}

void Button::setRotation(float _angleDeg)
{
    m_sprite.setRotation(_angleDeg);
    m_clickable.setBounds(m_sprite.getGlobalBounds());
    m_hoverable.setBounds(m_sprite.getGlobalBounds());
}

void Button::setClickCallback(component::Clickable::Callback _onClickCallback)
{
    m_clickable.setCallback(_onClickCallback);
    m_clickable.setBounds(m_sprite.getGlobalBounds());
}

void Button::onHandleEvent(const sf::Event& _event)
{
    switch (_event.type)
    {
    case sf::Event::MouseButtonReleased:
        m_clickable.onMouseReleased(_event.mouseButton);
        break;
    case sf::Event::MouseMoved:
        m_hoverable.onMouseMoved(_event.mouseMove);
        break;
    }
}

void Button::onDraw(sf::RenderWindow& _windowRef)
{
    _windowRef.draw(m_sprite);
}

} // namespace cn::menu::item
