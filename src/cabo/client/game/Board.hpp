#pragma once

#include "client/game/Card.hpp"
#include "client/game/Deck.hpp"
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

#include <deque>
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

    Board(const core::Context& _context, std::vector<game::Participant*>&& _participants, Deck& _deck, 
          menu::item::NotificationQueue& _queue, menu::item::Button& _finishButton, menu::item::Button& _caboButton,
          DecideActionButtons&& _decideButtons, DecideSwapButtons&& _decideSwapButtons, CardPositions _cardPositions);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);
    void update(sf::Time _dt);

    const core::Context& getContext() const { return m_contextRef; };
    Participant* getParticipant(PlayerId _id);
    PlayerId getLocalPlayerId() const { return m_localPlayerId; }

    CardPositions getCardPositions() const { return m_cardPositions; }

    bool canSayCabo() const;

    void showDecideActionButtons();
    void hideDecideActionButtons();

    void showDecideSwapButtons();
    void hideDecideSwapButtons();
    void showCaboButton();
    void hideCaboButton();
    void showFinishButton();
    void hideFinishButton();

    Card* drawCard(bool _fromDeck);
    Card* getDrawnCard() const { return m_drawnCardPtr; }
    void preDiscardCard(Card* _card);
    void discardCard(Card* _card);

    void fillNotificationQueue(const std::string& _message);

private:
    void changeStep(PlayerId _playerId, StepId _nextStepId, std::unique_ptr<Step> & _step);

    Card* getNextCardFromDeck();
    Card* getLastCardFromDiscard();

    const core::Context& m_contextRef;
    
    std::vector<Participant*> m_participants;
    Deck& m_deckRef;
    std::vector<Card*> m_discard;
    std::deque<Card*> m_cardsToDiscard;

    menu::item::NotificationQueue& m_queueRef;
    menu::item::Button& m_finishButtonRef;
    menu::item::Button& m_caboButtonRef;
    DecideActionButtons m_decideActionButtons;
    DecideSwapButtons m_decideSwapButtons;

    core::event::ListenerId m_listenerId{};
    PlayerId m_localPlayerId{};
    BoardState m_boardState = BoardState::Start;

    std::unordered_map<PlayerId, std::unique_ptr<Step>> m_steps;

    CardPositions m_cardPositions;

    bool m_isDealt = false;

    Card* m_drawnCardPtr;
};
   
} // namespace cn::client::game
