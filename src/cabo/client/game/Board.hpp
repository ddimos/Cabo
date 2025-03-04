#pragma once

#include "client/game/Participant.hpp"
#include "client/game/Step.hpp"
#include "client/menu/Types.hpp"
#include "client/menu/item/NotificationQueue.hpp"

#include "shared/player/Types.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/event/Types.hpp"
#include "core/object/Object.hpp"
#include "core/Context.hpp"

#include <SFML/System/Time.hpp>

#include <memory>
#include <vector>

namespace cn::client::game
{

class Board final
{
public:
    using DecideButtons = std::vector<client::menu::item::Button*>;

    Board(const core::Context& _context, std::vector<game::Participant*>&& _participants, menu::item::NotificationQueue& _queueRef,
          menu::item::Button& _finishButtonRef, DecideButtons&& _decideButtons);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);
    void update(sf::Time _dt);

    const core::Context& getContext() const { return m_contextRef; };
    Participant* getParticipant(PlayerId _id);

    void onParticipantStartDeciding();
    void onParticipantFinishDeciding();
    void onParticipantFinishesTurn(PlayerId _id);
    void onParticipantFinishedTurn(PlayerId _id);

private:
    void changeStep(StepId _nextStepId);

    const core::Context& m_contextRef;
    
    std::vector<Participant*> m_participants;
    menu::item::NotificationQueue& m_queueRef;
    menu::item::Button& m_finishButtonRef;
    DecideButtons m_decideButtons;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    PlayerId m_localPlayerId = PlayerIdInvalid;
    std::unique_ptr<Step> m_localPlayerStep = {}; //participantStep

    BoardState m_boardState = BoardState::Start;
    StepId m_localPlayerStepId = StepId::Idle;
    StepId m_desiredPlayerStepId = StepId::Idle;
};
   
} // namespace cn::client::game
