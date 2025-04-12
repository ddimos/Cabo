#pragma once

#include "client/game/Card.hpp"
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
#include <unordered_map>

namespace cn::client::game
{

class Board final
{
public:
    using DecideActionButtons = std::vector<client::menu::item::Button*>;
    using DecideSwapButtons = std::vector<client::menu::item::Button*>;

    Board(const core::Context& _context, std::vector<game::Participant*>&& _participants, std::vector<Card*>&& _cards, 
          menu::item::NotificationQueue& _queue, menu::item::Button& _finishButton,
          DecideActionButtons&& _decideButtons, DecideSwapButtons&& _decideSwapButtons);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);
    void update(sf::Time _dt);

    const core::Context& getContext() const { return m_contextRef; };
    Participant* getParticipant(PlayerId _id);
    PlayerId getLocalPlayerId() const { return m_localPlayerId; }

    void onParticipantStartDeciding(Card* _card);
    void onParticipantFinishDeciding();
    void onParticipantStartDecidingSwap();
    void onParticipantFinishDecidingSwap();
    void onParticipantFinishesTurn(PlayerId _id);
    void onParticipantFinishedTurn(PlayerId _id);

    void onShowMatchedCard(Card* _card);
    void onHideMatchedCard(bool _discard);

    Card* getDrawnCard() const { return m_drawnCardPtr; }

    void fillNotificationQueue(const std::string& _message);

private:
    void changeStep(PlayerId _playerId, StepId _nextStepId, std::unique_ptr<Step> & _step);

    Card* getNextCard();
    void discardCard(Card* _card);

    const core::Context& m_contextRef;
    
    std::vector<Participant*> m_participants;
    std::vector<Card*> m_cards;
    menu::item::NotificationQueue& m_queueRef;
    menu::item::Button& m_finishButtonRef;
    DecideActionButtons m_decideActionButtons;
    DecideSwapButtons m_decideSwapButtons;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    PlayerId m_localPlayerId = PlayerIdInvalid;
    BoardState m_boardState = BoardState::Start;

    std::unordered_map<PlayerId, std::unique_ptr<Step>> m_steps;

    bool m_isDealt = false;
    unsigned m_numberOfDiscardCards = 0;

    Card* m_drawnCardPtr;
    Card* m_matchedCardPtr;
};
   
} // namespace cn::client::game
