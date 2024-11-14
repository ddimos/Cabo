#include "game/Table.hpp"
#include "game/Constants.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


namespace cn::game
{

Table::Table(const sf::Texture& _tableTexture, DeckPtr _deck, unsigned _seed)
    : m_deck(_deck)
{
    m_sprite.setTexture(_tableTexture);
}

void Table::onUpdate(sf::Time _dt)
{
}

void Table::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::game