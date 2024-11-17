#include "game/Discard.hpp"
#include "core/Types.hpp"

#include "ResourceIds.hpp"

namespace cn::game
{

Discard::Discard(const core::Context& _context)
{
    m_sprite.setTexture(_context.textureHolderRef.get(TextureIds::Cards));
    m_sprite.setScale(0.4, 0.4);
    m_sprite.setPosition(650, 400);
}

void Discard::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::game
