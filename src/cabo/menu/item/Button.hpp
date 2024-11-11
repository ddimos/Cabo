#pragma once

#include "menu/Item.hpp"
#include "menu/component/Clickable.hpp"
#include "menu/component/Hoverable.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::menu::item
{

class Button : public Item
{
public:
    Button(Position _position,
        const sf::Texture& _texture,
        const sf::IntRect& _textureRectDefault,
        const sf::IntRect& _textureRectHover,
        component::Clickable::Callback _onClickCallback,
        sf::Mouse::Button _button,
        const sf::RenderWindow& _windowRef);
    
protected:
    void onHandleEvent(const sf::Event& _event) override;
    void onDraw(sf::RenderWindow& _windowRef) override;

    component::Clickable m_clickable;
    component::Hoverable m_hoverable;
    sf::Sprite m_sprite;

    sf::IntRect m_textureRectDefault = {};
    sf::IntRect m_textureRectHover = {};
};

} // namespace cn::menu::item
