#include "client/game/Deck.hpp"

#include "client/ResourceIds.hpp"

namespace cn::client::game
{

Deck::Deck(const core::Context& _context, shared::game::object::Id _id, unsigned _seed)
    : shared::game::object::Deck(_id, _seed)
{
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::Deck));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void Deck::onActivate()
{
    m_sprite.setPosition(getPosition());
}

void Deck::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::game
