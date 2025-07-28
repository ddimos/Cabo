#pragma once

#include "shared/board/controller/Controller.hpp"
#include "shared/board/component/Grabbable.hpp"
#include "shared/player/Types.hpp"

#include <unordered_map>

namespace cn::shared::board::controller
{

class Grabbable : public Controller<component::Grabbable>
{
public:
    Grabbable() = default;

    component::Grabbable* findObjectToGrab(PlayerId _playerId, sf::Vector2f _position);
    component::Grabbable* findObjectToRelease(PlayerId _playerId, sf::Vector2f _position);

    void grabObject(PlayerId _playerId, component::Grabbable& _component);
    void releaseObject(PlayerId _playerId, component::Grabbable& _component);

private:
    std::unordered_map<PlayerId, component::Grabbable*> m_grabbedObjects;
};

} // namespace cn::shared::game::controller
