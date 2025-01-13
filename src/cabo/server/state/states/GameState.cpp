#include "server/state/states/GameState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/GameEvents.hpp"
#include "events/SystemEvents.hpp"

#include "game/object/Card.hpp"
#include "game/object/Deck.hpp"
#include "game/object/Discard.hpp"
#include "game/object/Player.hpp"
#include "game/object/Table.hpp"
#include "game/Constants.hpp"

#include <ctime>

#include "core/Log.hpp"

namespace cn::server::states
{

GameState::GameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Game);

    unsigned seed = static_cast<unsigned>(std::time(nullptr));

    // std::vector<game::CardPtr> cards;
    // {
    //     unsigned short deckSize = game::StandartDeckSize;
    //     cards.reserve(deckSize);
    //     for (unsigned short i = 0; i < deckSize; ++i)
    //     {
    //         auto card = std::make_shared<game::Card>(cardPair.first, cardPair.second, image);
    //         card->setActivationOption(core::object::Object::ActivationOption::Manually);
    //         getContainer(core::object::Container::Type::Game).add(card);
    //         cards.push_back(card);
    //     }
    // }



    // getContainer(core::object::Container::Type::Game).add(table);
    // getContainer(core::object::Container::Type::Game).add(deck);
    // getContainer(core::object::Container::Type::Game).add(discard);

    m_listenerId = core::event::getNewListenerId();
}

void GameState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
  
    }
    else
    {

    }
}

core::state::Return GameState::onUpdate(sf::Time _dt)
{
    m_board->update(_dt);

    return core::state::Return::Break;
}

} // namespace cn::server::states
