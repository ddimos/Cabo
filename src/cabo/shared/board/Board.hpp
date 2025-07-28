#pragma once

#include "shared/board/Object.hpp"
#include "shared/board/Card.hpp"
#include "shared/board/Deck.hpp"
#include "shared/board/Discard.hpp"
#include "shared/board/Participant.hpp"
#include "shared/player/Types.hpp"
#include "core/Context.hpp"

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <vector>
#include <unordered_map>

namespace cn::shared::board
{

class Board
{
public:
    Board(unsigned _seed);

    Card& spawnCard();
    Deck& spawnDeck();
    Discard& spawnDiscard();
    Participant& spawnParticipant(PlayerId _playerId);

    void start();

    void participantGrabs(PlayerId _playerId, ObjectId _id, sf::Vector2f _position);
    void participantReleases(PlayerId _playerId, ObjectId _id, sf::Vector2f _position);
    void participantTurnsUp(PlayerId _playerId, ObjectId _id, sf::Vector2f _position);
    void participantTurnsDown(PlayerId _playerId, ObjectId _id, sf::Vector2f _position);
    void participantMoves(PlayerId _playerId, sf::Vector2f _position);

    Card* getCard(ObjectId _id);

private:
    ObjectId generateNextOjectId();

    const unsigned m_seed = 0;
    std::vector<std::unique_ptr<Card>> m_cards;
    std::unique_ptr<Deck> m_deck;
    std::unique_ptr<Discard> m_discard;
    std::unordered_map<PlayerId, std::unique_ptr<Participant>> m_participants;
    ObjectId::Type m_objectIdGenerator = 0;
};

} // namespace cn::shared::board
