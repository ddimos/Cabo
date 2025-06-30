#include "shared/board/GrabController.hpp"
#include "core/Assert.hpp"

namespace cn::shared::board
{

void GrabController::addObject(BoardObject& _object)
{
    m_objects.push_back(&_object);
}

BoardObject* GrabController::tryToGrabObject(PlayerId _playerId, sf::Vector2f _position)
{
    BoardObject* topObject = nullptr;
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

BoardObject* GrabController::tryToReleaseObject(PlayerId _playerId, sf::Vector2f)
{
    BoardObject* object = nullptr;
    auto it = m_grabbedObjects.find(_playerId);
    if (it != m_grabbedObjects.end())
    {
        // Don't check the position, release anyway
        it->second->onReleased();
        m_grabbedObjects.erase(it);
    }
    return object;
}

} // namespace cn::shared::board
