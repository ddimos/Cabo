#include "shared/game/object/Participant.hpp"
#include "shared/game/object/Object.hpp"

namespace cn::shared::game::object
{

Participant::Participant(Id _id, PlayerId _playerId)
    : Object(_id), m_playerId(_playerId)
{
}

void Participant::setMousePosition(sf::Vector2f _position)
{
    if (m_object)
        m_object->setPosition(_position);
}

void Participant::setObject(Object* _object)
{
    m_object = _object;
}

} // namespace cn::shared::game::object
