#pragma once

#include "core/event/Types.hpp"

#include "menu/Item.hpp"
#include "menu/component/Clickable.hpp"
#include "menu/component/Hoverable.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::menu::item
{

class Button : public Item
{
public:
    Button(const sf::Texture& _texture, const sf::IntRect& _textureRectDefault,
        const sf::IntRect& _textureRectHover, sf::Mouse::Button _button);
    Button(Position _position, const sf::Texture& _texture,
        const sf::IntRect& _textureRectDefault, const sf::IntRect& _textureRectHover,
        component::Clickable::Callback _onClickCallback, sf::Mouse::Button _button);

    Position getPosition() const;
    float getRotation() const;

    void setPosition(Position _position);
    void setRotation(float _angleDeg);
    void setClickCallback(component::Clickable::Callback _onClickCallback);
    
protected:
    void onDraw(sf::RenderWindow& _windowRef) override;
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    component::Clickable m_clickable;
    component::Hoverable m_hoverable;
    sf::Sprite m_sprite;

    sf::IntRect m_textureRectDefault = {};
    sf::IntRect m_textureRectHover = {};

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

} // namespace cn::menu::item
