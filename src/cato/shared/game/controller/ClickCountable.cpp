#include "shared/game/controller/ClickCountable.hpp"

namespace cn::shared::game::controller
{

component::ClickCountable* ClickCountable::findObjectToClick(PlayerId _playerId, sf::Vector2f _position)
{
    component::ClickCountable* topComponent = getTopObject(_position);
    return topComponent;
}

void ClickCountable::update(sf::Time _dt)
{
    for (auto it = m_clickedObjects.begin(); it != m_clickedObjects.end();)
    {
        (*it)->update(_dt);
        if ((*it)->timeoutReached() || (*it)->isClicked())
        {
            (*it)->reset();
            m_clickedObjects.erase(it);
        }
        else
            it++;
    }
}

void ClickCountable::clickObject(PlayerId _playerId, component::ClickCountable& _component)
{
    _component.click(_playerId);
    if (getClickedComponent(_component.getParent().getId()) == nullptr)
        m_clickedObjects.push_back(&_component);
}

component::ClickCountable* ClickCountable::clickObject(PlayerId _playerId, object::Id _id)
{
    auto* component = getObject(_id);
    CN_ASSERT(component);
    clickObject(_playerId, *component);
    return component;
}

component::ClickCountable* ClickCountable::getClickedComponent(object::Id _id)
{
    for (auto* obj : m_clickedObjects)
        if (obj->getParent().getId() == _id)
            return obj;
    return nullptr;
}

} // namespace cn::shared::game::controller
