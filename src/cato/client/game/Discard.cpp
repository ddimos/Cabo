#include "client/game/Discard.hpp"

#include "client/ResourceIds.hpp"

namespace cn::client::game
{

Discard::Discard(const core::Context& _context, shared::game::object::Id& _id)
    : shared::game::object::Discard(_id)
{
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::Discard));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void Discard::onActivate()
{
    m_sprite.setPosition(getPosition());
}

void Discard::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::game
