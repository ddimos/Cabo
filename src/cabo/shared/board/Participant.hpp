#pragma once

#include "shared/board/Object.hpp"
#include "shared/player/Types.hpp"

#include <SFML/System/Vector2.hpp>

namespace cn::shared::board
{

class Object;

class Participant : public Object
{
public:
    Participant(ObjectId _id, PlayerId _playerId);

    void setMousePosition(sf::Vector2f _position);
    void setObject(Object* _object);

private:
    PlayerId m_playerId{};
    sf::Vector2f m_mousePosition{};
    Object* m_object = nullptr;
};

} // namespace cn::shared::board
