#include "menu/item/Button.hpp"
#include "menu/Utils.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace cn::menu::item
{

Button::Button(Position _position, const sf::Texture& _texture, const sf::IntRect& _textureRectDefault,
    const sf::IntRect& _textureRectHover, component::Clickable::Callback _onClickCallback, sf::Mouse::Button _button, const sf::RenderWindow& _windowRef)
    : m_textureRectDefault(_textureRectDefault)
    , m_textureRectHover(_textureRectHover) 
{
    setPosition(_position);
    setTexture(_texture);
    setOnClickCallback(_onClickCallback);

    setTextureRect(m_textureRectDefault);
    setDrawPosition(calculateGlobalPos(getDrawBounds().getSize(), _windowRef.getSize()));
    setClickableBounds(getDrawBounds());
    setHoverableBounds(getDrawBounds());
    setClickableButton(_button);
    setOnHoverCallback(
        [this](bool _isHovered){
            if (_isHovered)
                setTextureRect(m_textureRectHover);
            else
                setTextureRect(m_textureRectDefault);
        }
    );
}

void Button::onHandleEvent(const sf::Event& _event)
{
    switch (_event.type)
    {
    case sf::Event::MouseButtonReleased:
        Clickable::onMouseReleased(_event.mouseButton);
        break;
    case sf::Event::MouseMoved:
        Hoverable::onMouseMoved(_event.mouseMove);
        break;
    }
}

void Button::onDraw(sf::RenderWindow& _windowRef)
{
    Drawable::draw(_windowRef);
}

} // namespace cn::menu::item
