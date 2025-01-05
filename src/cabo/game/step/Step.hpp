#pragma once

#include "core/event/Dispatcher.hpp"

#include <SFML/System/Time.hpp>

namespace cn::game::step
{

class Step
{
public:
    Step();
    virtual ~Step() = default;
    virtual void registerEvents(core::event::Dispatcher&, bool) {}
    virtual void update(sf::Time) {}

protected:
    core::event::ListenerId getListenerId() const { return m_listenerId; }
private:
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

// ------------------------------------------

class DrawFromDeck
{
public:
    DrawFromDeck();
  
};

// ------------------------------------------

class DecideDrawnCard
{
public:
    DecideDrawnCard();

};

// ------------------------------------------

class MatchCards
{
public:
    MatchCards();

};

// ------------------------------------------

class TakeCardInHand
{
public:
    TakeCardInHand();

};

// ------------------------------------------


// ------------------------------------------

class SeeFriendsCard
{
public:
    SeeFriendsCard();

};

} // namespace cn::game::step
