#include "client/menu/item/Button.hpp"
#include "client/menu/Utils.hpp"

#include "shared/events/InputEvents.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace cn::client::menu::item
{

Button::Button(const sf::Texture& _texture, const sf::IntRect& _textureRectDefault,
    const sf::IntRect& _textureRectHover, sf::Mouse::Button _button)
    : m_textureRectDefault(_textureRectDefault)
    , m_textureRectHover(_textureRectHover)
{
    m_sprite.setTexture(_texture);
    m_sprite.setTextureRect(m_textureRectDefault);
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

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
    m_sprite.setPosition(m_transformable.calculateGlobalPos(sf::Vector2f{}));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

    m_clickable.setCallback(_onClickCallback);
    m_clickable.setButton(_button);

    m_clickable.setBounds(sf::FloatRect(m_sprite.getPosition(), m_sprite.getLocalBounds().getSize()));
    m_hoverable.setBounds(sf::FloatRect(m_sprite.getPosition(), m_sprite.getLocalBounds().getSize()));

    m_hoverable.setCallback(
        [this](bool _isHovered){
            if (_isHovered)
                m_sprite.setTextureRect(m_textureRectHover);
            else
                m_sprite.setTextureRect(m_textureRectDefault);
        }
    );
}

float Button::getRotation() const
{
    return m_sprite.getRotation();
}

void Button::setRotation(float _angleDeg)
{
    m_sprite.setRotation(_angleDeg);
    m_hoverable.setRotation(_angleDeg);
    m_clickable.setRotation(_angleDeg);

    m_clickable.setBounds(sf::FloatRect(m_sprite.getPosition(), m_sprite.getLocalBounds().getSize()));
    m_hoverable.setBounds(sf::FloatRect(m_sprite.getPosition(), m_sprite.getLocalBounds().getSize()));
}

void Button::setClickCallback(component::Clickable::Callback _onClickCallback)
{
    m_clickable.setCallback(_onClickCallback);
}

void Button::setTextureRectPosition(sf::Vector2i _posDefault, sf::Vector2i _posHover)
{
    m_textureRectDefault = sf::IntRect(_posDefault, m_textureRectDefault.getSize());
    m_textureRectHover = sf::IntRect(_posHover, m_textureRectHover.getSize());
    m_sprite.setTextureRect(m_textureRectDefault);
}

void Button::onDraw(sf::RenderWindow& _windowRef)
{
    _windowRef.draw(m_sprite);
}

void Button::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        if (m_listenerId == core::event::ListenerIdInvalid)
            m_listenerId = core::event::getNewListenerId();

        _dispatcher.registerEvent<events::MouseButtonReleasedEvent>(m_listenerId,
            [this](const events::MouseButtonReleasedEvent& _event){
                m_clickable.onMouseReleased(_event.mouseButton);
            }
        );
        _dispatcher.registerEvent<events::MouseMovedEvent>(m_listenerId,
            [this](const events::MouseMovedEvent& _event){
                m_hoverable.onMouseMoved(_event.mouseMove);
            }
        );
    }
    else
    {
        if (m_listenerId == core::event::ListenerIdInvalid)
            return;

        _dispatcher.unregisterEvent<events::MouseButtonReleasedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::MouseMovedEvent>(m_listenerId);
    }
}

void Button::onPositionUpdated()
{
    m_sprite.setPosition(m_transformable.calculateGlobalPos(sf::Vector2f{}));
    m_clickable.setBounds(sf::FloatRect(m_sprite.getPosition(), m_sprite.getLocalBounds().getSize()));
    m_hoverable.setBounds(sf::FloatRect(m_sprite.getPosition(), m_sprite.getLocalBounds().getSize()));
}

} // namespace cn::client::menu::item
