#include "game/step/SeeOwnCard.hpp"
#include "game/object/Card.hpp"
#include "game/object/Player.hpp"
#include "events/GameEvents.hpp"


namespace cn::game::step
{

SeeOwnCard::SeeOwnCard(Board& _board)
    : m_boardRef(_board)
{
}

void SeeOwnCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::LocalPlayerClickSlotEvent>(getListenerId(),
            [this](const events::LocalPlayerClickSlotEvent& _event)
            {    
                if (m_state != State::WaitInput)
                    return;
                m_nextState = State::RequestSeeCard;
                m_playerId = _event.playerId;
                m_slotId = _event.slotId;
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(getListenerId());
    }
}

void SeeOwnCard::update(sf::Time _dt)
{
    if (m_nextState != m_state)
    {
        // on enter
        switch (m_nextState)
        {
        case State::WaitInput:
            break;
        case State::RequestSeeCard:
            // send a request to the server
            break;
        case State::SeeCard:
        {
            auto player = m_boardRef.getPlayer(m_playerId);
            player->showCardInSlot(m_slotId);
            break;
        }
        case State::Finished:
        {
            auto player = m_boardRef.getPlayer(m_playerId);
            player->hideCardInSlot(m_slotId);
            break;
        }
        }
        m_state = m_nextState;
    }

    if (m_state == State::RequestSeeCard)
        m_nextState = State::SeeCard;
    else if (m_state == State::SeeCard)
    {
        m_seeCardTimeDt -= _dt;
        if (m_seeCardTimeDt.asSeconds() <= 0.f)
            m_nextState = State::Finished;
    }
    else if (m_state == State::Finished)
    {
        m_nextState = State::WaitInput;
        m_seeCardTimeDt = m_seeCardTime;
    }
}

} // namespace cn::game::step
