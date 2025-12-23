#include "shared/game/object/Participant.hpp"
#include "shared/game/object/Object.hpp"
#include "shared/game/object/Card.hpp"

namespace cn::shared::game::object
{

Participant::Participant(Id _id, PlayerId _playerId)
    : Object(_id), m_playerId(_playerId), m_layerableComponent(*this)
{
}

void Participant::setMousePosition(sf::Vector2f _position)
{
    if (m_object)
        static_cast<Card*>(m_object)->move(_position);
}

void Participant::setObject(Object* _object)
{
    m_object = _object;
}

} // namespace cn::shared::game::object
