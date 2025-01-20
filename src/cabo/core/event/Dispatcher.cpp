#include "core/event/Dispatcher.hpp"
#include "core/Assert.hpp"

namespace cn::core::event
{

ListenerId getNewListenerId()
{
    static ListenerId listenerId = 0;
    return listenerId++;
}

void Dispatcher::processDelayed()
{
    while (!m_delayedEvents.empty())
    {
        auto& eventPtr = m_delayedEvents.front();
        auto eventIt = m_callbacks.find(eventPtr->getId());
        if (eventIt != m_callbacks.end())
        {
            for(auto& listeners : eventIt->second)
                listeners.second->call(*eventPtr);
        }
            
        m_delayedEvents.pop();
    }
}

Dispatcher::Listeners& Dispatcher::getOrCreateListeners(EventId _eventId)
{
    auto eventIt = m_callbacks.find(_eventId);
    if (eventIt == m_callbacks.end())
    {
        auto [iter, inserted] = m_callbacks.emplace(_eventId, Listeners{});
        CN_ASSERT(inserted);
        eventIt = iter;
    }
    return eventIt->second;
}

void Dispatcher::unregisterEvent(ListenerId _listenerId, EventId _eventId)
{
    auto eventIt = m_callbacks.find(_eventId);
    if (eventIt == m_callbacks.end())
        return;

    eventIt->second.erase(_listenerId);
}

void Dispatcher::sendDelayed(EventPtr&& _event)
{
    m_delayedEvents.emplace(std::move(_event));
}

} // namespace cn::core::event
