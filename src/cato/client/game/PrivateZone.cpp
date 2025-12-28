#include "client/game/PrivateZone.hpp"

#include "client/ResourceIds.hpp"

namespace cn::client::game
{

PrivateZone::PrivateZone(const core::Context& _context, shared::game::object::Id _id, PlayerId _ownerId, const std::string& _name)
    : shared::game::object::PrivateZone(_id, _ownerId)
{
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::PrivateZone));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

    m_name.setFont(_context.get<FontHolder>().get(FontIds::Main));
    m_name.setCharacterSize(15);
    m_name.setFillColor(sf::Color::Black);
    m_name.setString(_name);
    m_name.setOrigin(m_name.getLocalBounds().getSize() / 2.f);
}

void PrivateZone::onActivate()
{
    m_sprite.setPosition(getPosition());
    m_sprite.setRotation(getRotation());

    m_name.setPosition(getPosition());
    m_name.setRotation(getRotation());
}

void PrivateZone::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
    _window.draw(m_name);
}

} // namespace cn::client::game
