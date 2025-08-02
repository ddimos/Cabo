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
    if (getBoardCard().getFlippableComponent().isFaceUp())
    {
        if (getBoardCard().getValue().isValid())
        {
            auto card = shared::game::getCardFromValue(getBoardCard().getValue());
            m_sprite.setTextureRect(game::spriteSheet::getCardTextureRect(card.first, card.second));
        }
        else
        {
            m_sprite.setTextureRect(game::spriteSheet::getBlurredCardTextureRect());
        }
    }
    else
    {
        m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
    }
    m_sprite.setPosition(getBoardCard().getPosition());
}

void Card::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::game
