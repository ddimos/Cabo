#include "shared/board/Board.hpp"

#include "core/math/Math.hpp"
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
    m_deck = std::make_unique<shared::board::Deck>(generateNextOjectId(), m_seed, sf::Vector2f(400.f, 400.f));
    return *m_deck;
}

Discard& Board::spawnDiscard()
{
    m_discard = std::make_unique<shared::board::Discard>(generateNextOjectId());
    return *m_discard;
}

Participant& Board::spawnParticipant(PlayerId _playerId)
{
    auto [it, inserted] = m_participants.try_emplace(_playerId, std::make_unique<shared::board::Participant>(generateNextOjectId(), _playerId));
    CN_ASSERT(inserted);
    return *(it->second);
}

Anchor& Board::spawnAnchor()
{
    constexpr int radius = 100;
    m_anchors.emplace_back(std::make_unique<shared::board::Anchor>(generateNextOjectId(), radius));
    m_anchors.back()->setPosition(sf::Vector2f(500.f, 500.f));
    return *(m_anchors.back());
}

void Board::start()
{
    m_deck->shuffle();
    auto points = core::math::generatePointOnEllipse(780, 460, m_participants.size(), {780.f, 460.f});
    unsigned i = 0;
    for (const auto& [id, part] : m_participants)
    {
        part->setPosition(points.at(i).first);
        part->setRotation(points.at(i).second);
        ++i;
    }
}

void Board::participantGrabs(PlayerId _playerId, ObjectId _id, sf::Vector2f _position)
{
    CN_LOG_FRM("Grabs {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_participants.at(_playerId)->setObject(card);
    m_participants.at(_playerId)->setMousePosition(_position);
}

void Board::participantReleases(PlayerId _playerId, ObjectId _id, sf::Vector2f _position)
{
    CN_LOG_FRM("Releases {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_participants.at(_playerId)->setMousePosition(_position);
    m_participants.at(_playerId)->setObject(nullptr);
}

void Board::participantTurnsUp(PlayerId _playerId, ObjectId _id, sf::Vector2f _position)
{
    CN_LOG_FRM("TurnsUp {} {}", _playerId.value(), _id.value());
}

void Board::participantTurnsDown(PlayerId _playerId, ObjectId _id, sf::Vector2f _position)
{
    CN_LOG_FRM("TurnsDown {} {}", _playerId.value(), _id.value());
}

void Board::participantMoves(PlayerId _playerId, sf::Vector2f _position)
{
    CN_LOG_FRM("Moves {} {}:{}", _playerId.value(), _position.x, _position.y);
    m_participants.at(_playerId)->setMousePosition(_position);
}

ObjectId Board::generateNextOjectId()
{
    ObjectId newId(m_objectIdGenerator++);
    CN_ASSERT(newId.isValid());
    return newId;
}

Card* Board::getCard(ObjectId _id)
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
