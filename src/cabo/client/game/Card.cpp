#include "client/game/Card.hpp"

namespace cn::client::game
{

Card::Card(Rank _rank, Suit _suit, client::menu::item::SimpleImage& _image)
    : shared::game::Card(_rank, _suit), m_image(_image)
{
}

void Card::setPosition(sf::Vector2f _position)
{
    m_image.setPosition(client::menu::Position{ .m_position = _position });
}

void Card::setRotation(float _angleDeg)
{
    m_image.setRotation(_angleDeg);
}

void Card::onActivate()
{
    m_image.requestActivated();    // TODO add the children concept on the object level
}

void Card::onDeactivate()
{
    m_image.requestDeactivated();
}

} // namespace cn::client::game
