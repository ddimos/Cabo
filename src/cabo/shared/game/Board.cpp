#include "shared/game/Board.hpp"
#include "shared/game/Constants.hpp"
#include "shared/game/Placement.hpp"
#include "shared/Types.hpp"

#include "core/math/Math.hpp"
#include "core/Assert.hpp"
#include "core/Log.hpp"

namespace cn::shared::game
{

Board::Board(
    const std::vector<Player>& _players,
    std::function<object::Card*(object::Id)> _createCardFunc,
    std::function<object::Deck*(object::Id)> _createDeckFunc,
    std::function<object::Discard*(object::Id)> _createDiscardFunc,
    std::function<object::Participant*(object::Id, PlayerId)> _createParticipantFunc,
    std::function<object::PrivateZone*(object::Id, PlayerId)> _createPrivateZoneFunc,
    std::function<object::CountableButton*(object::Id, TableButtonType, unsigned)> _createButtonFunc
)
    : m_layerController({
            { layer::Cards, StandartDeckSize },
            { layer::GrabbedCards, MaxNumberOfPlayers * 2},
            { layer::Players, MaxNumberOfPlayers}
        })
{
    {
        m_deck = _createDeckFunc(generateNextOjectId());
        m_deck->setPosition(placement::getDeck().pos);
        m_deck->setSize({70.f, 100.f});
    }
    {
        m_discard = _createDiscardFunc(generateNextOjectId());
        m_discard->setPosition(placement::getDiscard().pos);
        m_discard->setSize({70.f, 100.f});
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

    // TODO auto points = core::math::generatePointOnEllipse(900, 480, _players.size(), {960.f, 540.f});
    
    auto points = placement::getParticipant(_players.size());
    unsigned i = 0;
    for (const Player& player : _players)
    {
        auto [it, inserted] = m_participants.try_emplace(player.id, _createParticipantFunc(generateNextOjectId(), player.id));
        CN_ASSERT(inserted);
        it->second->setPosition(points.at(i).pos);
        it->second->setRotation(points.at(i).rot);
        m_layerController.addTolayer(layer::Players, it->second->getLayerableComponent());

        auto* zone = m_privateZones.emplace_back(_createPrivateZoneFunc(generateNextOjectId(), player.id));
        zone->setPosition(points.at(i).pos);
        zone->setRotation(points.at(i).rot);
        zone->setSize(sf::Vector2f(160.f, 120.f));

        ++i;
    }

    {
        unsigned numberOfPlayerToClick = (_players.size() == 1) ? 1 : 2;
        auto* button = _createButtonFunc(generateNextOjectId(), TableButtonType::Deal, numberOfPlayerToClick);
        button->setPosition(placement::getButton(TableButtonType::Deal).pos);
        button->setSize(sf::Vector2f(40.f, 20.f));
        m_clickCountableController.add(button->getClickCountableComponent());
        
        button = _createButtonFunc(generateNextOjectId(), TableButtonType::ResetAndShuffle, numberOfPlayerToClick);
        button->setPosition(placement::getButton(TableButtonType::ResetAndShuffle).pos);
        button->setSize(sf::Vector2f(40.f, 20.f));
        m_clickCountableController.add(button->getClickCountableComponent());
        
        button = _createButtonFunc(generateNextOjectId(), TableButtonType::ShuffleFromDiscard, numberOfPlayerToClick);
        button->setPosition(placement::getButton(TableButtonType::ShuffleFromDiscard).pos);
        button->setSize(sf::Vector2f(40.f, 20.f));
        m_clickCountableController.add(button->getClickCountableComponent());
    }
}

void Board::start(const std::vector<object::Card::Value>& _cardValues)
{
    m_deck->shuffle();
    m_deck->visit(
        [this](object::Card& _card){
            m_layerController.addTolayer(layer::Cards, _card.getLayerableComponent());
        }
    );

    CN_ASSERT(_cardValues.empty() || _cardValues.size() == m_cards.size());
    for (size_t i = 0; i < _cardValues.size(); ++i)
        m_cards.at(i)->setValue(_cardValues.at(i));
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

void Board::update(sf::Time _dt)
{
    m_clickCountableController.update(_dt);
}

void Board::participantGrabs(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("Grabs {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_participants.at(_playerId)->setObject(card);
    m_participants.at(_playerId)->setMousePosition(_position);
    m_layerController.removeFromLayer(layer::Cards, card->getLayerableComponent());
    m_layerController.addTolayer(layer::GrabbedCards, card->getLayerableComponent());
}

void Board::participantReleases(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("Releases {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_participants.at(_playerId)->setObject(nullptr);
    m_layerController.removeFromLayer(layer::GrabbedCards, card->getLayerableComponent());
    m_layerController.addTolayer(layer::Cards, card->getLayerableComponent());
    sf::Vector2f pos = _position;
    if (m_discard->contains(_position) && !card->isInDiscard())
    {
        m_discard->discard(card);
        pos = m_discard->getPosition();
    }
    if (m_deck->contains(_position) && !card->isInDeck())
    {
        m_deck->add(*card);
        pos = m_deck->getPosition();
    }
    card->release(pos);
}

object::Object* Board::participantClicks(PlayerId _playerId, sf::Vector2f _position)
{
    auto* component = m_clickCountableController.findObjectToClick(_playerId, _position);
    if (!component)
        return nullptr;
    m_clickCountableController.clickObject(_playerId, *component);
    auto& object = component->getParent();
    CN_LOG_I_FRM("Clicks {} {}", _playerId.value(), object.getId().value());
    return &(object);
}
// TODO think how to improve this part with having two similar methods 
void Board::participantClicks(PlayerId _playerId, object::Id _id)
{
    m_clickCountableController.clickObject(_playerId, _id);
    CN_LOG_I_FRM("Clicks {}", _playerId.value());
}

void Board::participantTurnsUp(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("TurnsUp {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_layerController.promoteInLayer(
        card->getGrabbableComponent().isGrabbed() ? layer::GrabbedCards : layer::Cards,
        card->getLayerableComponent()
    );
}

void Board::participantTurnsDown(PlayerId _playerId, object::Id _id, sf::Vector2f _position)
{
    CN_LOG_I_FRM("TurnsDown {} {}", _playerId.value(), _id.value());
    auto* card = getCard(_id);
    m_layerController.promoteInLayer(
        card->getGrabbableComponent().isGrabbed() ? layer::GrabbedCards : layer::Cards, 
        card->getLayerableComponent()
    );
}

void Board::participantMoves(PlayerId _playerId, sf::Vector2f _position)
{
    CN_LOG_D_FRM("Moves {} {}:{}", _playerId.value(), _position.x, _position.y);
    m_participants.at(_playerId)->setMousePosition(_position);
}

} // namespace cn::shared::game
