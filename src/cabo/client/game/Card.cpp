#include "client/game/Card.hpp"
#include "client/game/SpriteSheet.hpp"
#include "client/ResourceIds.hpp"

#include "shared/game/CardValue.hpp"

namespace cn::client::game
{

Card::Card(const core::Context& _context, shared::game::object::Id _id) 
    : shared::game::object::Card(_id)
    , m_interpolatedPos(sf::seconds(0.3f), core::Easing::linear)
    , m_interpolatedFlip(sf::seconds(1.f), core::Easing::easeInOutExpo)
{
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::Cards));
    m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void Card::startTransit(sf::Vector2f _pos)
{
    m_interpolatedPos.start(getPosition(), _pos);
}

void Card::startFlipping(bool _wantsToBeUp)
{
    if (_wantsToBeUp == m_wantsToBeUp)
        return;

    m_wantsToBeUp = _wantsToBeUp;
    m_interpolatedFlip.start(-1.f, 1.f);
}

void Card::onUpdate(sf::Time _dt)
{
    // TODO don't set textures each frame
    if (m_interpolatedFlip.doesInterpolate())
    {
        if (m_interpolatedFlip.get() <= 0.0f)
        {
            if (m_wantsToBeUp)
            {
                m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
            }
            else
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
        }
        else
        {
            if (m_wantsToBeUp)
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
        }
        float value = m_interpolatedFlip.get();
        m_sprite.setScale(value < 0.f ? -value : value, m_sprite.getScale().y);
    }
    else
    {
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
    }

    if (m_interpolatedPos.doesInterpolate())
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
