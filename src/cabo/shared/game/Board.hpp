#pragma once

#include "shared/game/object/Anchor.hpp"
#include "shared/game/object/Object.hpp"
#include "shared/game/object/Card.hpp"
#include "shared/game/object/Deck.hpp"
#include "shared/game/object/Discard.hpp"
#include "shared/game/object/Participant.hpp"
#include "shared/game/object/PrivateZone.hpp"
#include "shared/game/controller/Layerable.hpp"
#include "shared/player/Types.hpp"
#include "core/Context.hpp"

#include <SFML/System/Vector2.hpp>

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

namespace cn::shared::game
{

class Board
{
public:
    Board(const std::vector<Player>& _players,
        std::function<object::Card*(object::Id)> _createCardFunc,
        std::function<object::Deck*(object::Id)> _createDeckFunc,
        std::function<object::Discard*(object::Id)> _createDiscardFunc,
        std::function<object::Participant*(object::Id, PlayerId)> _createParticipantFunc,
        std::function<object::PrivateZone*(object::Id, PlayerId)> _createPrivateZoneFunc
    );

    void start(const std::vector<object::Card::Value>& _cardValues);

    void participantGrabs(PlayerId _playerId, object::Id _id, sf::Vector2f _position);
    void participantReleases(PlayerId _playerId, object::Id _id, sf::Vector2f _position);
    void participantTurnsUp(PlayerId _playerId, object::Id _id, sf::Vector2f _position);
    void participantTurnsDown(PlayerId _playerId, object::Id _id, sf::Vector2f _position);
    void participantMoves(PlayerId _playerId, sf::Vector2f _position);

    object::Card* getCard(object::Id _id);
    object::Participant* getParticipant(PlayerId _playerId);

private:
    object::Id generateNextOjectId();

    std::vector<object::Card*> m_cards;
    object::Deck* m_deck;
    object::Discard* m_discard;
    std::unordered_map<PlayerId, object::Participant*> m_participants;
    std::vector<object::PrivateZone*> m_privateZones;
    std::vector<object::Anchor*> m_anchors;
    object::Id::Type m_objectIdGenerator = 0;
    shared::game::controller::Layerable m_layerController;
};

} // namespace cn::shared::game
