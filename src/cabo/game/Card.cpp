#include "game/Card.hpp"

#include "menu/item/SimpleImage.hpp"

namespace cn::game
{

Card::Card(Rank _rank, Suit _suit, menu::item::SimpleImagePtr _image)
    : m_rank(_rank), m_suit(_suit), m_image(_image)
{
}

void Card::deal(/*player id*/ /*position*/)
{
}

void Card::discard()
{
}

void Card::setPosition(sf::Vector2f _position)
{
    m_image->setPosition(cn::menu::Position{ .m_position = _position });
}

void Card::onActivate()
{
    m_image->activate();    // TODO add the children concept on the object level
}

void Card::onDeactivate()
{
    m_image->deactivate();
}

} // namespace cn::game
