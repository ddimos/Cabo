#pragma once

#include "menu/Item.hpp"
#include <SFML/Graphics/Text.hpp>

#include <string>

namespace cn::menu::item
{

class SimpleText : public Item
{
public:
    SimpleText(Position _position, 
        const std::string& _text,
        const sf::Font& _font,
        unsigned _fontSize, 
        sf::Color _color,
        const sf::RenderWindow& _windowRef);

    void updateText(const std::string& _text, const sf::RenderWindow& _windowRef);

private:
    void onDraw(sf::RenderWindow& _window) override;
    
    sf::Text m_text;
};
} // namespace cn::menu::item
