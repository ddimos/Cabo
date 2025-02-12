#include "client/state/states/GameState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleImage.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "client/player/Manager.hpp"

#include "client/game/Participant.hpp"
#include "client/game/SpriteSheet.hpp"
#include "client/game/Table.hpp"
#include "client/game/Types.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/InputEvents.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <ctime>

#include "core/Log.hpp"

namespace cn::client::states
{

GameState::GameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);
    createContainer(core::object::Container::Type::Game);

    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& textureHolderRef = getContext().get<TextureHolder>();
    auto& fontHolderRef = getContext().get<FontHolder>();
    // auto& netManRef = getContext().get<net::Manager>();
    auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
    auto& playerManagerRef = getContext().get<player::Manager>();


    auto table = std::make_shared<game::Table>(getContext());
    getContainer(core::object::Container::Type::Game).add(table);
    auto spawnPoints = table->generateSpawnPoints(playerManagerRef.getPlayers().size(), sf::Vector2f(windowRef.getSize()) / 2.f);

    std::vector<game::Participant*> participants;
    {
        
        const auto& players = playerManagerRef.getPlayers();
        size_t playerIndex = playerManagerRef.getIndexOfLocalPlayer();
        size_t countLeft = players.size();
        size_t index = 0;
        do
        {
            const cn::Player& player = players[playerIndex];

            auto playerId = player.id;
            std::map<game::ParticipantSlotId, game::ParticipantSlot> slots;
            unsigned short numberOfSlots = shared::game::MaxNumberOfParticipantSlots;
            for (game::ParticipantSlotId slotId = 0; slotId < numberOfSlots; ++slotId)
            {
                // TODO make a choosable button?
                auto slotButton = std::make_shared<menu::item::Button>(
                    menu::Position{},
                    textureHolderRef.get(TextureIds::Cards),
                    game::spriteSheet::getCardBackTextureRect(),
                    game::spriteSheet::getCardBackTextureRect(game::spriteSheet::Hover::Yes),
                    [this, slotId, playerId](){
                        auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
                        eventDispatcherRef.send<events::LocalPlayerClickSlotEvent>(slotId, playerId);
                    },
                    sf::Mouse::Button::Left
                );
                slotButton->setActivationOption(core::object::Object::ActivationOption::Manually);

                getContainer(core::object::Container::Type::Menu).add(slotButton);
                slots.emplace(slotId, game::ParticipantSlot{ slotId, nullptr, slotButton.get() });
            }

            auto participant = std::make_shared<game::Participant>(
                getContext(), playerId, true, std::move(slots), shared::game::DefaultInitNumberOfParticipantSlots
            );
            participant->setSpawnPoint(spawnPoints[index]);
            getContainer(core::object::Container::Type::Game).add(participant);

            participants.push_back(participant.get());

            playerIndex++;
            if (playerIndex >= players.size())
                playerIndex = 0;
            
            index++;
            countLeft--;
        }
        while (countLeft != 0);

    }


    m_listenerId = core::event::getNewListenerId();
}

void GameState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::KeyReleasedEvent>(m_listenerId,
            [this](const events::KeyReleasedEvent& _event){

            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::KeyReleasedEvent>(m_listenerId);
    }

}

core::state::Return GameState::onUpdate(sf::Time _dt)
{

    return core::state::Return::Break;
}

} // namespace cn::client::states
