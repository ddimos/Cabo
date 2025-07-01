#include "shared/board/Board.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

namespace cn::shared::board
{

Board::Board(unsigned _seed)
    : m_seed(_seed)
{
}

Card& Board::spawnCard()
{
    m_cards.emplace_back(std::make_unique<shared::board::Card>(generateNextOjectId()));
    return  *(m_cards.back());
}

Deck& Board::spawnDeck()
{
    m_deck = std::make_unique<shared::board::Deck>(m_seed, sf::Vector2f(400.f, 400.f));
    return *m_deck;
}

Discard& Board::spawnDiscard()
{
    m_discard = std::make_unique<shared::board::Discard>();
    return *m_discard;
}

Participant& Board::spawnParticipant(PlayerId _playerId)
{
    auto [it, inserted] = m_participants.try_emplace(_playerId, std::make_unique<shared::board::Participant>(_playerId));
    CN_ASSERT(inserted);
    return *(it->second);
}

void Board::start()
{
    m_deck->shuffle();
}

void Board::participantGrabs(PlayerId _playerId, BoardObjectId _id, sf::Vector2f _position)
{
    CN_LOG_FRM("Grabs {} {}", _playerId.value(), _id.value());
    m_participants.at(_playerId)->setObject(getCard(_id));
    m_participants.at(_playerId)->setMousePosition(_position);
}

void Board::participantReleases(PlayerId _playerId, BoardObjectId _id, sf::Vector2f _position)
{
    CN_LOG_FRM("Releases {} {}", _playerId.value(), _id.value());
    m_participants.at(_playerId)->setMousePosition(_position);
    m_participants.at(_playerId)->setObject(nullptr);
}

void Board::participantMoves(PlayerId _playerId, sf::Vector2f _position)
{
    CN_LOG_FRM("Moves {} {}:{}", _playerId.value(), _position.x, _position.y);
    m_participants.at(_playerId)->setMousePosition(_position);
}

BoardObjectId Board::generateNextOjectId()
{
    BoardObjectId newId(m_objectIdGenerator++);
    CN_ASSERT(newId.isValid());
    return newId;
}

BoardObject* Board::getCard(BoardObjectId _id)
{
    for (auto& card : m_cards)
    {
        if (card->getId() == _id)
            return card.get();
    }
    CN_ASSERT_FRM(false, "No card with id: {}", _id.value());
    return nullptr;
}

} // namespace cn::shared::board
