#include "shared/board/InteractableController.hpp"
#include "core/Assert.hpp"

namespace cn::shared::board
{

void InteractableController::addObject(InteractableObject& _object)
{
    m_objects.push_back(&_object);
}

InteractableObject* InteractableController::tryToGrabObject(PlayerId _playerId, sf::Vector2f _position)
{
    InteractableObject* topObject = getTopObject(_position);

    if (topObject && !topObject->isGrabbed())
    {
        CN_ASSERT(!m_grabbedObjects.contains(_playerId));
        m_grabbedObjects[_playerId] = topObject;
    }
    return topObject;
}

InteractableObject* InteractableController::tryToReleaseObject(PlayerId _playerId, sf::Vector2f)
{
    InteractableObject* object = nullptr;
    auto it = m_grabbedObjects.find(_playerId);
    if (it != m_grabbedObjects.end())
    {
        // Don't check the position, release anyway
        object = it->second;
        m_grabbedObjects.erase(it);
    }
    return object;
}

InteractableObject* InteractableController::getTopObject(sf::Vector2f _position)
{
    InteractableObject* topObject = nullptr;
    for (auto& object : m_objects)
    {
        if (!object->contains(_position))
            continue;
        
        if (!topObject)
        {
            topObject = object;
            continue;
        }

        if (topObject->getZIndex() < object->getZIndex())
            topObject = object;
    }
    return topObject;
}

} // namespace cn::shared::board
