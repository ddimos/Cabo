#include "menu/item/SimpleText.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::menu::item
{

SimpleText::SimpleText(Position _position, 
        const std::string& _text,
        const sf::Font& _font,
        unsigned _fontSize, 
        sf::Color _color,
        const sf::RenderWindow& _windowRef)
{
    m_transformable.setPosition(_position);

    m_text.setFont(_font);
    m_text.setString(_text);
    m_text.setFillColor(_color);
    m_text.setCharacterSize(_fontSize);
    m_text.setPosition(m_transformable.calculateGlobalPos(m_text.getGlobalBounds().getSize(), _windowRef.getSize()));
}

void SimpleText::updateText(const std::string& _text, const sf::RenderWindow& _windowRef)
{
    m_text.setString(_text);
    m_text.setPosition(m_transformable.calculateGlobalPos(m_text.getGlobalBounds().getSize(), _windowRef.getSize()));
}

void SimpleText::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_text);
}

} // namespace cn::menu::item
