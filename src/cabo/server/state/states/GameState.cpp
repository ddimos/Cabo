#include "server/state/states/GameState.hpp"
#include "server/state/StateIds.hpp"

#include "server/game/Board.hpp"
#include "server/game/Card.hpp"
#include "server/game/Deck.hpp"
#include "server/game/Discard.hpp"
#include "server/game/Participant.hpp"
#include "server/game/Types.hpp"

#include "server/player/Manager.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/events/GameEvents.hpp"
#include "shared/net/Manager.hpp"
#include "shared/Types.hpp"

#include "core/Log.hpp"

namespace cn::server::states
{

GameState::GameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Game);

    // auto& netManagerRef = getContext().get<net::Manager>();
    auto& playerManagerRef = getContext().get<player::Manager>();
    unsigned seed = getContext().get<shared::Seed>().seed;


    std::vector<shared::game::Card*> cards;
    {
        unsigned short deckSize = shared::game::StandartDeckSize;
        cards.reserve(deckSize);
        for (unsigned short i = 0; i < deckSize; ++i)
        {
            auto cardPair = game::Card::getCardFromIndex(i);
            auto card = std::make_shared<game::Card>(cardPair.first, cardPair.second);
            card->setId(game::CardId(static_cast<uint8_t>(i)));
            getContainer(core::object::Container::Type::Game).add(card);
            cards.push_back(card.get());
        }
    }

    auto deck = std::make_shared<game::Deck>(
        std::move(cards), seed
    );

    auto discard = std::make_shared<game::Discard>();

    // // // getContainer(core::object::Container::Type::Game).add(table);
    getContainer(core::object::Container::Type::Game).add(deck);
    getContainer(core::object::Container::Type::Game).add(discard);

    std::vector<game::Participant*> participants;
    for (const auto& player : playerManagerRef.getPlayers())
    {
        unsigned short numberOfSlots = shared::game::MaxNumberOfParticipantSlots;
        
        std::vector<game::ParticipantSlot> slots;
        slots.reserve(numberOfSlots);

        for (game::ParticipantSlotId::Type slotId = 0; slotId < numberOfSlots; ++slotId)
        {
            slots.emplace_back(game::ParticipantSlot{ game::ParticipantSlotId(slotId), false, nullptr });
        }
        auto playerId = player.id;
        auto participant = std::make_shared<game::Participant>(
            getContext(), playerId, std::move(slots), shared::game::DefaultInitNumberOfParticipantSlots
        );
        getContainer(core::object::Container::Type::Game).add(participant);
        
        participants.push_back(participant.get());
    }
    // TODO to randomize for the first play, then the player who won  
    PlayerId firstParticipantTurn = playerManagerRef.getPlayers().front().id;
    
    m_board = std::make_unique<game::Board>(getContext(), *deck, *discard, std::move(participants), firstParticipantTurn);

    m_listenerId = core::event::getNewListenerId();

    CN_LOG("Game state..");
}

void GameState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    m_board->registerEvents(_dispatcher, _isBeingRegistered);
}

core::state::Return GameState::onUpdate(sf::Time _dt)
{
    m_board->update(_dt);

    return core::state::Return::Break;
}

} // namespace cn::server::states
