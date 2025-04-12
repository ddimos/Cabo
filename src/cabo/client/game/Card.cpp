#include "client/game/Card.hpp"
#include "client/game/SpriteSheet.hpp"

#include "client/ResourceIds.hpp"

#include "core/Assert.hpp"
#include "core/math/Math.hpp"

namespace cn::client::game
{

Card::Card(const core::Context& _context, sf::Vector2f _position)
    : m_contextRef(_context)
{
    m_sprite.setPosition(_position);
    m_sprite.setTexture(m_contextRef.get<TextureHolder>().get(TextureIds::Cards));
    m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

    m_transitionSpeed = 450.f;
}

void Card::set(Rank _rank, Suit _suit)
{
    shared::game::Card::set(_rank, _suit);
}

void Card::changeState(ChangeStateParams _params)
{
    m_params = _params;
    m_sprite.setRotation(m_params.desiredRotation);
    enterTransitingState();
}

void Card::show(bool _visible)
{
    if (_visible)
        m_sprite.setTextureRect(game::spriteSheet::getCardTextureRect(getRank(), getSuit()));
    else
        m_sprite.setTextureRect(game::spriteSheet::getBlurredCardTextureRect());
}

void Card::hide()
{
    m_sprite.setTextureRect(game::spriteSheet::getCardBackTextureRect());
}

void Card::onUpdate(sf::Time _dt)
{
    if (m_state == State::Transiting)
    {
        m_sprite.move(m_transitionSpeed * m_direction * _dt.asSeconds());

        if (core::math::vectorLength(m_sprite.getPosition() - m_initPosition) >= m_distance)
        {
            m_sprite.setPosition(m_params.desiredPosition);
            m_state = m_params.desiredState;
            m_params.onFinishCallback();
        }
    }
}

void Card::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

void Card::enterTransitingState()
{
    CN_ASSERT(m_state != State::Transiting);

    m_state = State::Transiting;
    
    m_initPosition = m_sprite.getPosition();
    m_direction = m_params.desiredPosition - m_initPosition;
    m_distance = core::math::vectorLength(m_direction);
    m_direction = core::math::normalizeVector(m_direction);
}

} // namespace cn::client::game
