#include "shared/board/GrabController.hpp"
#include "core/Assert.hpp"

namespace cn::shared::board
{

void GrabController::addObject(Object& _object)
{
    m_objects.push_back(&_object);
}

Object* GrabController::tryToGrabObject(PlayerId _playerId, sf::Vector2f _position)
{
    Object* topObject = nullptr;
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

    if (topObject && !topObject->isGrabbed())
    {
        CN_ASSERT(!m_grabbedObjects.contains(_playerId));

        topObject->onGrabbed();
        m_grabbedObjects[_playerId] = topObject;
    }
    return topObject;
}

Object* GrabController::tryToReleaseObject(PlayerId _playerId, sf::Vector2f)
{
    Object* object = nullptr;
    auto it = m_grabbedObjects.find(_playerId);
    if (it != m_grabbedObjects.end())
    {
        // Don't check the position, release anyway
        it->second->onReleased();
        object = it->second;
        m_grabbedObjects.erase(it);
    }
    return object;
}

} // namespace cn::shared::board
