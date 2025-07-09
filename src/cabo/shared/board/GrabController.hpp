#pragma once

#include "shared/player/Types.hpp"
#include "shared/board/Object.hpp"

#include <SFML/System/Vector2.hpp>

#include <unordered_map>
#include <vector>

namespace cn::shared::board
{

class GrabController
{
public:
    GrabController() = default;
   
    void addObject(Object& _object);

    Object* tryToGrabObject(PlayerId _playerId, sf::Vector2f _position);
    Object* tryToReleaseObject(PlayerId _playerId, sf::Vector2f _position);

private:
    std::vector<Object*> m_objects;
    std::unordered_map<PlayerId, Object*> m_grabbedObjects;
};

} // namespace cn::shared::game
