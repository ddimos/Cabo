#include "shared/board/Participant.hpp"
#include "shared/board/Object.hpp"

namespace cn::shared::board
{

Participant::Participant(ObjectId _id, PlayerId _playerId)
    : Object(_id), m_playerId(_playerId)
{
}

void Participant::setMousePosition(sf::Vector2f _position)
{
    m_mousePosition = _position;
    if (m_object)
        m_object->setPosition(_position);
}

void Participant::setObject(Object* _object)
{
    m_object = _object;
}

} // namespace cn::shared::board
