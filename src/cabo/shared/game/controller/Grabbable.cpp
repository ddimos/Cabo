#include "shared/game/controller/Grabbable.hpp"
#include "core/Assert.hpp"

namespace cn::shared::game::controller
{

component::Grabbable* Grabbable::findObjectToGrab(PlayerId _playerId, sf::Vector2f _position)
{
    CN_ASSERT(!m_grabbedObjects.contains(_playerId));
    component::Grabbable* topComponent = getTopObject(_position);
    if (topComponent && !topComponent->isGrabbed())
        return topComponent;
    return nullptr;
}

component::Grabbable* Grabbable::findObjectToRelease(PlayerId _playerId, sf::Vector2f)
{
    auto it = m_grabbedObjects.find(_playerId);
    if (it != m_grabbedObjects.end())
    {
        // Don't check the position, release anyway
        return it->second;
    }
    return nullptr;
}

void Grabbable::grabObject(PlayerId _playerId, component::Grabbable& _component)
{
    CN_ASSERT(!m_grabbedObjects.contains(_playerId));
    CN_ASSERT(!_component.isGrabbed());
    m_grabbedObjects[_playerId] = &_component;
    _component.grab();
}

void Grabbable::releaseObject(PlayerId _playerId, component::Grabbable& _component)
{
    auto it = m_grabbedObjects.find(_playerId);
    CN_ASSERT(it != m_grabbedObjects.end());
    m_grabbedObjects.erase(it);
    _component.release();
}

} // namespace cn::shared::game::controller
