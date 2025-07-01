#pragma once

#include "shared/player/Types.hpp"

#include <SFML/System/Vector2.hpp>

namespace cn::shared::board
{

class BoardObject;

class Participant final
{
public:
    Participant(PlayerId _playerId);

    void setMousePosition(sf::Vector2f _position);
    void setObject(BoardObject* _object);

private:
    PlayerId m_playerId{};
    sf::Vector2f m_mousePosition{};
    BoardObject* m_object = nullptr;
};

} // namespace cn::shared::board
