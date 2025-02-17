#include "client/game/Board.hpp"
#include "client/game/step/SeeOwnCard.hpp"
#include "client/game/Types.hpp"

#include "client/player/Manager.hpp"

#include "shared/events/NetworkEvents.hpp"

#include "core/Log.hpp"

namespace cn::client::game
{

Board::Board(const core::Context& _context, std::vector<game::Participant*>&& _participants)
    : m_contextRef(_context)
    , m_participants(std::move(_participants))
{
    auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
    auto& playerManagerRef = getContext().get<player::Manager>();

    m_localPlayerId = playerManagerRef.getLocalPlayerId();

    m_listenerId = core::event::getNewListenerId();
}

void Board::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::PlayerStepUpdateEvent>(m_listenerId,
            [&_dispatcher, this](const events::PlayerStepUpdateEvent& _event){
                CN_LOG_FRM("Stepp {} layer {}", (unsigned)_event.m_stepId, _event.m_playerId);
                if (_event.m_playerId != m_localPlayerId)
                {
                    // TODO show a notification
                    return;
                }
                switch (_event.m_stepId)
                {
                case StepId::SeeOwnCard:
                    m_localPlayerStep = std::make_unique<step::SeeOwnCard>(*this, m_localPlayerId);
                    break;
                    
                default:
                    break;
                }
                m_localPlayerStep->registerEvents(_dispatcher, true);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::PlayerStepUpdateEvent>(m_listenerId);
    }
}

void Board::update(sf::Time _dt)
{
    if (m_localPlayerStep)
        m_localPlayerStep->update(_dt);
}

Participant* Board::getParticipant(PlayerId _id)
{
    for (auto* participant : m_participants)
    {
        if (participant->getId() == _id)
            return participant;
    }
    CN_ASSERT_FRM(false, "Couldn't find a participant {}", _id);
    return nullptr;
}

} // namespace cn::client::game
