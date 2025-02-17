#include "client/state/states/GameState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleImage.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "client/player/Manager.hpp"

#include "client/game/Card.hpp"
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

                auto cardPair = shared::game::Card::getCardFromIndex(0);
                auto cardImage = std::make_shared<menu::item::SimpleImage>(
                    menu::Position{},
                    textureHolderRef.get(TextureIds::Cards),
                    game::spriteSheet::getCardTextureRect(cardPair.first, cardPair.second)
                );
                cardImage->setActivationOption(core::object::Object::ActivationOption::Manually);
                getContainer(core::object::Container::Type::Menu).add(cardImage);

                slots.emplace(slotId, game::ParticipantSlot{ slotId, game::Card{cardPair.first, cardPair.second}, cardImage.get(), slotButton.get(), false});
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

    auto deckButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(75.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        },
        textureHolderRef.get(TextureIds::Cards),
        game::spriteSheet::getCardBackTextureRect(),
        game::spriteSheet::getCardBackTextureRect(game::spriteSheet::Hover::Yes),
        [this, &eventDispatcherRef](){
            eventDispatcherRef.send<events::LocalPlayerClickDeckEvent>();
        },
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(deckButton);

    auto discardButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(-75.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        },
        textureHolderRef.get(TextureIds::Cards),
        game::spriteSheet::getDiscardTextureRect(),
        game::spriteSheet::getDiscardTextureRect(game::spriteSheet::Hover::Yes),
        [this, &eventDispatcherRef](){
            eventDispatcherRef.send<events::LocalPlayerClickDiscardEvent>();
        },
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(discardButton);

    m_board = std::make_unique<game::Board>(
        getContext(), std::move(participants)
    );

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

    m_board->registerEvents(_dispatcher, _isBeingRegistered);
}

core::state::Return GameState::onUpdate(sf::Time _dt)
{
    m_board->update(_dt);

    return core::state::Return::Break;
}

} // namespace cn::client::states
