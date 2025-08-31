#include "shared/game/Board.hpp"
#include "shared/game/Constants.hpp"

#include "core/math/Math.hpp"
#include "core/Assert.hpp"
#include "core/Log.hpp"

namespace cn::shared::game
{

Board::Board(const std::vector<Player>& _players,
        std::function<object::Card*(object::Id)> _createCardFunc,
        std::function<object::Deck*(object::Id)> _createDeckFunc,
        std::function<object::Discard*(object::Id)> _createDiscardFunc,
        std::function<object::Participant*(object::Id, PlayerId)> _createParticipantFunc,
        std::function<object::PrivateZone*(object::Id, PlayerId)> _createPrivateZoneFunc)
{
    {
        m_deck = _createDeckFunc(generateNextOjectId());
        m_deck->setPosition(sf::Vector2f(400.f, 400.f));
    }
    {
        m_discard = _createDiscardFunc(generateNextOjectId());
        m_discard->setPosition(sf::Vector2f(700.f, 400.f));
    }
    {
        unsigned short deckSize = game::StandartDeckSize;
        for (unsigned short i = 0; i < deckSize; ++i)
        {
            auto* card = _createCardFunc(generateNextOjectId());
            card->setSize({70.f, 100.f});
            m_cards.push_back(card);
        }
        m_deck->setCards(m_cards);
    }

    for (const Player& player : _players)
    {
        auto [it, inserted] = m_participants.try_emplace(player.id, _createParticipantFunc(generateNextOjectId(), player.id));
        CN_ASSERT(inserted);

        m_privateZones.emplace_back(_createPrivateZoneFunc(generateNextOjectId(), player.id));
        m_privateZones.back()->setSize(sf::Vector2f(400.f, 200.f));
    }
}

void Board::start(const std::vector<object::Card::Value>& _cardValues)
{
    m_deck->shuffle();

    CN_ASSERT(_cardValues.empty() || _cardValues.size() == m_cards.size());
    for (size_t i = 0; i < _cardValues.size(); ++i)
        m_cards.at(i)->setValue(_cardValues.at(i));

    auto points = core::math::generatePointOnEllipse(780, 460, m_participants.size(), {780.f, 460.f});
    unsigned i = 0;
    for (const auto& [id, part] : m_participants)
    {
        part->setPosition(points.at(i).first);
        part->setRotation(points.at(i).second);
        m_privateZones.at(i)->setPosition(points.at(i).first);
        // m_privateZones.at(i)->setRotation(points.at(i).second); TODO
        ++i;
    }
}

object::Id Board::generateNextOjectId()
{
    object::Id newId(m_objectIdGenerator++);
    CN_ASSERT(newId.isValid());
    return newId;
}

object::Card* Board::getCard(object::Id _id)
{
    for (auto* card : m_cards)
    {
        if (card->getId() == _id)
            return card;
    }
    CN_ASSERT_FRM(false, "No card with id: {}", _id.value());
    return nullptr;
}

object::Participant* Board::getParticipant(PlayerId _playerId)
{
    CN_ASSERT_FRM(m_participants.contains(_playerId), "No participant with id: {}", _playerId.value());
    return m_participants.at(_playerId);
}

void Board::participantGrabs(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("Grabs {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_participants.at(_playerId)->setObject(card);
    m_participants.at(_playerId)->setMousePosition(_position);
}

void Board::participantReleases(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("Releases {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_participants.at(_playerId)->setObject(nullptr);
}

void Board::participantTurnsUp(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("TurnsUp {} {}", _playerId.value(), _id.value());
}

void Board::participantTurnsDown(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("TurnsDown {} {}", _playerId.value(), _id.value());
}

void Board::participantMoves(PlayerId _playerId, sf::Vector2f _position)
{
    CN_LOG_D_FRM("Moves {} {}:{}", _playerId.value(), _position.x, _position.y);
    m_participants.at(_playerId)->setMousePosition(_position);
}

} // namespace cn::shared::game
