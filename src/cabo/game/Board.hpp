#pragma once

#include "core/event/Dispatcher.hpp"
#include "core/object/Object.hpp"
#include "core/Types.hpp"

#include "game/Types.hpp"
#include "menu/Types.hpp"

#include <SFML/System/Time.hpp>

#include <vector>

namespace cn::game
{

class Board
{
    enum class State
    {
        None,
        LookingCard,
        Playing,
        Finishing
    };
    enum class PlayerTurn
    {
        DrawCard,
        DecideAction,
        Match,
        Take,
        CardAction,
        End
    };
public:
    using DecideButtons = std::vector<menu::item::ButtonPtr>;

    Board(const core::Context& _context, DeckPtr _deck, DiscardPtr _discard, TablePtr _table,
        DecideButtons&& _decideButtons);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    void start();
    bool hasGameStarted() const;

    PlayerPtr getPlayer(PlayerId _id) const;
    
    // TODO replace with events
    void addPlayer(PlayerPtr _player);
    void onLocalPlayerClickDeck();
    void onLocalPlayerClickDiscard();
    void onLocalPlayerClickDecideButton(unsigned _butonIndex);
    void onLocalPlayerClickSlot(PlayerSlotId _slotId, Player& _player);

private:
    void onUpdate(sf::Time _dt);

    const core::Context& m_contextRef;
    DeckPtr m_deck;
    DiscardPtr m_discard;
    TablePtr m_table;
    std::vector<PlayerPtr> m_players;

    DecideButtons m_decideButtons;

    unsigned m_activePlayerIndex = 0;
    CardPtr m_currentCard;

    State m_playingState = State::None;
    PlayerTurn m_playerTurn = PlayerTurn::DrawCard;
};

} // namespace cn::game
