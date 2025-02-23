#pragma once

#include "client/menu/Item.hpp"
#include <SFML/Graphics/Text.hpp>

#include <string>

namespace cn::client::menu::item
{

class SimpleText : public Item
{
public:
    SimpleText(Position _position, const std::string& _text,
        const sf::Font& _font, unsigned _fontSize, sf::Color _color);

    Position getPosition() const;
    
    void setPosition(Position _position);
    void updateText(const std::string& _text);

private:
    void onDraw(sf::RenderWindow& _window) override;
    
    sf::Text m_text;
};
} // namespace cn::client::menu::item
