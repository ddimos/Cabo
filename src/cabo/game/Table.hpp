#pragma once

#include "core/object/Object.hpp"

#include "game/Card.hpp"
#include "game/Types.hpp"

#include "menu/Types.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

namespace cn::core
{
    struct Context;
}

namespace cn::game
{

class Table final : public core::object::Object
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

    Table(const core::Context& _context, DeckPtr _deck, DiscardPtr _discard, 
        DecideButtons&& _decideButtons);

    void addPlayer(PlayerPtr _player);

    void start();
    bool hasGameStarted() const;
    
    void onLocalPlayerClickDeck();
    void onLocalPlayerClickDiscard();
    void onLocalPlayerClickDecideButton(unsigned _butonIndex);
    void onLocalPlayerClickSlot(PlayerSlotId _slotId, Player& _player);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;


    sf::Sprite m_sprite;

    const core::Context& m_contextRef;
    DeckPtr m_deck;
    DiscardPtr m_discard;
    std::vector<PlayerPtr> m_players;
    std::vector<PlayerSpawnPoint> m_spawnPoints;

    DecideButtons m_decideButtons;

    unsigned m_activePlayerIndex = 0;
    CardPtr m_currentCard;

    State m_playingState = State::None;
    PlayerTurn m_playerTurn = PlayerTurn::DrawCard;
};

} // namespace cn::game
