#pragma once

#include "shared/player/Types.hpp"
#include "shared/board/Object.hpp"

#include <SFML/System/Vector2.hpp>

#include <unordered_map>
#include <vector>

namespace cn::shared::board
{

class InteractableController
{
public:
    InteractableController() = default;
   
    void addObject(InteractableObject& _object);

    InteractableObject* tryToGrabObject(PlayerId _playerId, sf::Vector2f _position);
    InteractableObject* tryToReleaseObject(PlayerId _playerId, sf::Vector2f _position);

    InteractableObject* getTopObject(sf::Vector2f _position);

private:
    std::vector<InteractableObject*> m_objects;
    std::unordered_map<PlayerId, InteractableObject*> m_grabbedObjects;
};

} // namespace cn::shared::game
