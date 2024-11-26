#pragma once

#include "core/event/Event.hpp"
#include "core/event/Slot.hpp"
#include "core/event/Types.hpp"
#include "core/Assert.hpp"

#include <queue>
#include <unordered_map>

namespace cn::core::event
{

ListenerId getNewListenerId();

class Manager;

class Dispatcher final
{
public:
    template<typename TEvent>
    void registerEvent(ListenerId _listenerId, Slot<TEvent>::Callback _callback);
    template<typename TEvent>
    void unregisterEvent(ListenerId _listenerId);

    void send(EventPtr&& _event);

private:
    friend class Manager;
    using Listeners = std::unordered_map<ListenerId, SlotBasePtr>;

    Listeners& getOrCreateListeners(EventId _eventId);
    void unregisterEvent(ListenerId _listenerId, EventId _eventId);
    
    void process();

    std::unordered_map<EventId, Listeners> m_callbacks;
    std::queue<EventPtr> m_events;
};

template<typename TEvent>
void Dispatcher::registerEvent(ListenerId _listenerId, Slot<TEvent>::Callback _callback)
{
    EventId eventId = TEvent::getIdStatic();

    Listeners& listeners = getOrCreateListeners(eventId);
    auto listIt = listeners.find(_listenerId);
    if (listIt != listeners.end())
    {
        CN_ASSERT(false);
        return;
    }
    listeners.emplace(_listenerId, std::make_unique<Slot<TEvent>>(_callback));
}

template<typename TEvent>
void Dispatcher::unregisterEvent(ListenerId _listenerId)
{
    EventId eventId = TEvent::getIdStatic();
    unregisterEvent(_listenerId, eventId);
}

} // namespace cn::core::event
