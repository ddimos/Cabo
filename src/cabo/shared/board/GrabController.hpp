#pragma once

#include "shared/player/Types.hpp"
#include "shared/board/BoardObject.hpp"

#include <SFML/System/Vector2.hpp>

#include <unordered_map>
#include <vector>

namespace cn::shared::board
{

class GrabController
{
public:
    GrabController() = default;
   
    void addObject(BoardObject& _object);

    BoardObject* tryToGrabObject(PlayerId _playerId, sf::Vector2f _position);
    BoardObject* tryToReleaseObject(PlayerId _playerId, sf::Vector2f _position);

private:
    std::vector<BoardObject*> m_objects;
    std::unordered_map<PlayerId, BoardObject*> m_grabbedObjects;
};

} // namespace cn::shared::game
