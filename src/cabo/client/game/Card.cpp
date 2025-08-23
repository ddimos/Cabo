#include "client/game/Card.hpp"
#include "client/game/SpriteSheet.hpp"
#include "client/ResourceIds.hpp"

#include "shared/game/CardValue.hpp"

namespace cn::client::game
{

Card::Card(const core::Context& _context, shared::game::object::Id _id) 
    : shared::game::object::Card(_id)
    , m_interpolatedPos(_context.get<sf::Clock>(), sf::seconds(0.3f), core::Easing::linear)
    , m_interpolatedFlip(_context.get<sf::Clock>(), sf::seconds(1.f), core::Easing::linear)
{
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::Cards));
    m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void Card::startTransit(sf::Vector2f _pos)
{
    m_interpolatedPos.start(getPosition(), _pos);
}

void Card::startFlipping()
{
    m_startFlipping = true;
    m_isNextFaceUp = !getFlippableComponent().isFaceUp();
    m_interpolatedFlip.start(1.f);
}

void Card::onUpdate(sf::Time _dt)
{
    // if (m_startFlipping)
    // {
    //     if (m_interpolatedFlip.get() < 0.5f)
    //     {
    //         if (m_isNextFaceUp)
    //         {
    //             m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
    //         }
    //         else
    //         {
    //             if (getBoardCard().getValue().isValid())
    //             {
    //                 auto card = shared::game::getCardFromValue(getBoardCard().getValue());
    //                 m_sprite.setTextureRect(game::spriteSheet::getCardTextureRect(card.first, card.second));
    //             }
    //             else
    //             {
    //                 m_sprite.setTextureRect(game::spriteSheet::getBlurredCardTextureRect());
    //             }
    //         }

    //     }
    //     else
    //     {
    //         if (m_isNextFaceUp)
    //         {
    //             if (getBoardCard().getValue().isValid())
    //             {
    //                 auto card = shared::game::getCardFromValue(getBoardCard().getValue());
    //                 m_sprite.setTextureRect(game::spriteSheet::getCardTextureRect(card.first, card.second));
    //             }
    //             else
    //             {
    //                 m_sprite.setTextureRect(game::spriteSheet::getBlurredCardTextureRect());
    //             }
    //         }
    //         else
    //         {
    //             m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
    //         }
    //     }
    //     m_sprite.setScale(m_interpolatedFlip.get() ,m_sprite.getScale().y);
    // }
    if (getFlippableComponent().isFaceUp())
    {
        if (getValue().isValid())
        {
            auto card = shared::game::getCardFromValue(getValue());
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
    if (!m_interpolatedPos.isFinished())
    {
        auto pos = m_interpolatedPos.get();
        m_sprite.setPosition(pos);
        setPosition(pos);
    }
    else
    {
        m_sprite.setPosition(getPosition());
    }
}

void Card::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::game
