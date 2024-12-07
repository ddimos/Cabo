#include "game/Board.hpp"
#include "game/object/Deck.hpp"
#include "game/object/Discard.hpp"
#include "game/object/Player.hpp"
#include "game/object/Table.hpp"
#include "game/Constants.hpp"

#include "core/Assert.hpp"
#include "core/Types.hpp"

#include "menu/item/Button.hpp"
#include "menu/Utils.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::game
{

Board::Board(const core::Context& _context, DeckPtr _deck, DiscardPtr _discard, TablePtr _table,
        DecideButtons&& _decideButtons)
    : m_contextRef(_context)
    , m_deck(_deck)
    , m_discard(_discard)
    , m_table(_table)
    , m_decideButtons(std::move(_decideButtons))
{
    unsigned index = 0;
    for (auto& button : m_decideButtons)
    {
        button->setPosition(cn::menu::Position{ .m_position = sf::Vector2f(780, 510 + index * 22) });
        button->setClickCallback([this, index](bool _isPressed){
            onLocalPlayerClickDecideButton(index);
        });
        ++index;
    }
}

void Board::onUpdate(sf::Time _dt)
{
    if (m_playerTurn == PlayerTurn::End)
    {
        m_playerTurn = PlayerTurn::DrawCard;
    }
}

void Board::start()
{
    CN_ASSERT(!hasGameStarted());

    for (auto& player : m_players)
    {
        unsigned short numberOfCards = DefaultInitialNumberOfCards;
        while (numberOfCards)
        {
            // TODO if not empty
            player->deal(m_deck->getNextCard());
            --numberOfCards;
        }
    }

    m_playingState = State::LookingCard;
}

bool Board::hasGameStarted() const
{
    return m_playingState != State::None;
}

void Board::addPlayer(PlayerPtr _player)
{
    m_players.push_back(_player);
    auto spawnPoints = m_table->generateSpawnPoints(m_players.size(), sf::Vector2f(m_contextRef.windowRef.getSize()) / 2.f);

    int i = 0;
    for (auto& player : m_players)
    {
        player->setSpawnPoint(spawnPoints.at(i));
        ++i;
    }
}

void Board::onLocalPlayerClickDeck()
{
    if (!hasGameStarted())
        return;
    
    if (m_playerTurn != PlayerTurn::DrawCard)
        return;
    m_playerTurn = PlayerTurn::DecideAction;

    // TODO if not empty
    auto card = m_deck->getNextCard();
    card->setPosition(sf::Vector2f(850, 500));
    card->requestActivated();

    m_currentCard = card;

    for (auto& button : m_decideButtons)
        button->requestActivated();
}

void Board::onLocalPlayerClickDiscard()
{

}

void Board::onLocalPlayerClickDecideButton(unsigned _butonIndex)
{
    if (m_playerTurn != PlayerTurn::DecideAction)
    {
        CN_ASSERT(false);
        return;
    }
    for (auto& button : m_decideButtons)
        button->requestDeactivated();

    switch (_butonIndex)
    {
    case 0: // Match
        m_playerTurn = PlayerTurn::Match;
        // Highlight player cards
        break;
    case 1: // Take
        m_playerTurn = PlayerTurn::Take;
        // Highlight player cards
        break;
    case 2: // Action
        if (!Card::hasAbility(*m_currentCard))
            return;
        m_playerTurn = PlayerTurn::CardAction;
        break;
    case 3: // Discard
        m_discard->discard(m_currentCard);
        m_currentCard.reset();
        m_playerTurn = PlayerTurn::End;
        break;
    default:
        CN_ASSERT(false);
        break;
    }
}

void Board::onLocalPlayerClickSlot(PlayerSlotId _slotId, Player& _player)
{
    if (m_playerTurn == PlayerTurn::CardAction)
    {
        // TODO
    }
    else if (m_playerTurn == PlayerTurn::Match)
    {
        if (!_player.isLocal())
            return;

        auto card = _player.get(_slotId);
        if (card->getRank() == m_currentCard->getRank())
        {
            m_discard->discard(m_currentCard);
            card->requestActivated();
            m_discard->discard(card);
            m_currentCard.reset();
            _player.removeSlot(_slotId);

        }
        else
        {
            card->requestActivated();
            _player.addSlot();
            _player.deal(m_deck->getNextCard());
        }
        m_playerTurn = PlayerTurn::End;
    } 
    else if (m_playerTurn == PlayerTurn::Take)
    {
        if (!_player.isLocal())
            return;
        
        auto prevCard = _player.replace(_slotId, m_currentCard);
        m_currentCard->requestDeactivated();
        m_currentCard.reset();
        prevCard->requestActivated();
        m_discard->discard(prevCard);
        m_playerTurn = PlayerTurn::End;
    }
    else
    {
        CN_ASSERT(false);
    }
}

} // namespace cn::game
