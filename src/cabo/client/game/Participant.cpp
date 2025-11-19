#include "client/game/Participant.hpp"
#include "shared/game/Constants.hpp"

#include "client/ResourceIds.hpp"

namespace cn::client::game
{

Participant::Participant(const core::Context& _context, shared::game::object::Id _id, PlayerId _playerId, bool _isLocal)
    : shared::game::object::Participant(_id, _playerId)
    , m_isLocal(_isLocal)
    , m_interpolatorPos(shared::game::MoveUpdateDuration, core::Easing::linear)
{
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::Paw));
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize().x / 2.f, 0.f);
    m_sprite.scale(0.75f, 0.75f);
}

void Participant::onActivate()
{
    m_sprite.setRotation(getRotation());
}

void Participant::setMousePosition(sf::Vector2f _pos)
{
    if (m_isLocal)
    {
        shared::game::object::Participant::setMousePosition(_pos);
        m_sprite.setPosition(_pos);
    }
    else
        m_interpolatorPos.start(_pos);
}

void Participant::onUpdate(sf::Time) 
{
    if (m_isLocal)
        return;

    auto pos = sf::Vector2f(m_interpolatorPos.get());
    shared::game::object::Participant::setMousePosition(pos);
    m_sprite.setPosition(pos);
}

void Participant::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::game
