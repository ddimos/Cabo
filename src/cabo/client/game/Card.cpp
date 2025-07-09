#include "client/game/Card.hpp"
#include "client/game/SpriteSheet.hpp"

#include "client/ResourceIds.hpp"

namespace cn::client::game
{

Card::Card(const core::Context& _context, shared::board::Card& _boardCard) 
    : shared::game::Card(_boardCard)
{
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::Cards));
    m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void Card::onUpdate(sf::Time _dt)
{
    m_sprite.setPosition(m_boardCardRef.getPosition());
}
    
void Card::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::game
