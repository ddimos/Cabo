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

    template<typename TEvent, typename... Args>
    void send(Args... _args);
    template<typename TEvent, typename... Args>
    void sendDelayed(Args... _args);
    void sendDelayed(EventPtr&& _event);

private:
    friend class Manager;
    using Listeners = std::unordered_map<ListenerId, SlotBasePtr>;

    void processDelayed();

    Listeners& getOrCreateListeners(EventId _eventId);
    void unregisterEvent(ListenerId _listenerId, EventId _eventId);

    std::unordered_map<EventId, Listeners> m_callbacks;
    std::queue<EventPtr> m_delayedEvents;
};

template<typename TEvent>
void Dispatcher::registerEvent(ListenerId _listenerId, Slot<TEvent>::Callback _callback)
{
    CN_ASSERT(_listenerId != ListenerIdInvalid);

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
    CN_ASSERT(_listenerId != ListenerIdInvalid);
    
    EventId eventId = TEvent::getIdStatic();
    unregisterEvent(_listenerId, eventId);
}

template<typename TEvent, typename... Args>
void Dispatcher::send(Args... _args)
{
    EventId eventId = TEvent::getIdStatic();
    auto eventIt = m_callbacks.find(eventId);
    if (eventIt != m_callbacks.end())
    {
        TEvent event(_args...);
        for(auto& listener : eventIt->second)
            static_cast<Slot<TEvent>*>(listener.second.get())->callSpecific(event);
    }
}

template<typename TEvent, typename... Args>
void Dispatcher::sendDelayed(Args... _args)
{
    sendDelayed(std::move(std::make_unique<TEvent>(_args...)));
}

} // namespace cn::core::event
