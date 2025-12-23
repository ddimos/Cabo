#pragma once

#include "shared/game/controller/Controller.hpp"
#include "shared/game/component/ClickCountable.hpp"
#include "shared/player/Types.hpp"

#include <vector>

namespace cn::shared::game::controller
{

class ClickCountable : public Controller<component::ClickCountable>
{
public:
    ClickCountable() = default;

    component::ClickCountable* findObjectToClick(PlayerId _playerId, sf::Vector2f _position);

    void update(sf::Time _dt);
    void clickObject(PlayerId _playerId, component::ClickCountable& _component);
    component::ClickCountable* clickObject(PlayerId _playerId, object::Id _id);

private:
    component::ClickCountable* getClickedComponent(object::Id _id);

    std::vector<component::ClickCountable*> m_clickedObjects;
};

} // namespace cn::shared::game::controller
