#pragma once

#include "client/game/Card.hpp"
#include "client/game/Participant.hpp"
#include "client/game/Step.hpp"
#include "client/menu/Types.hpp"
#include "client/menu/item/NotificationQueue.hpp"
#include "client/menu/item/SimpleImage.hpp"

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
    using DecideActionButtons = std::vector<client::menu::item::Button*>;
    using DecideSwapButtons = std::vector<client::menu::item::Button*>;

    Board(const core::Context& _context, std::vector<game::Participant*>&& _participants, menu::item::NotificationQueue& _queue,
          menu::item::Button& _finishButton, menu::item::SimpleImage& _deckCardImage, menu::item::SimpleImage& _discardCardImage,
          menu::item::SimpleImage& _matchedCardImageRef, DecideActionButtons&& _decideButtons, DecideSwapButtons&& _decideSwapButtons);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);
    void update(sf::Time _dt);

    const core::Context& getContext() const { return m_contextRef; };
    Participant* getParticipant(PlayerId _id);

    void onParticipantStartDeciding(Card _card);
    void onParticipantFinishDeciding();
    void onParticipantStartDecidingSwap();
    void onParticipantFinishDecidingSwap();
    void onParticipantFinishesTurn(PlayerId _id);
    void onParticipantFinishedTurn(PlayerId _id);

    void onShowMatchedCard(Card _card);
    void onHideMatchedCard(bool _discard);

    Card getDrawnCard() const { return m_drawnCard; }

private:
    void changeStep(StepId _nextStepId);
    void discardCard(Card _card);

    const core::Context& m_contextRef;
    
    std::vector<Participant*> m_participants;
    menu::item::NotificationQueue& m_queueRef;
    menu::item::Button& m_finishButtonRef;
    menu::item::SimpleImage& m_deckCardImageRef;
    menu::item::SimpleImage& m_discardCardImageRef;
    menu::item::SimpleImage& m_matchedCardImageRef;
    DecideActionButtons m_decideActionButtons;
    DecideSwapButtons m_decideSwapButtons;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    PlayerId m_localPlayerId = PlayerIdInvalid;
    std::unique_ptr<Step> m_localPlayerStep = {}; //participantStep
    
    BoardState m_boardState = BoardState::Start;
    StepId m_localPlayerStepId = StepId::Idle;
    StepId m_desiredPlayerStepId = StepId::Idle;

    unsigned m_numberOfDiscardCards = 0;

    Card m_drawnCard;
    Card m_matchedCard;
};
   
} // namespace cn::client::game
