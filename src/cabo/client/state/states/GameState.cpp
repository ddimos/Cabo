#include "client/state/states/GameState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "client/player/Manager.hpp"

#include "client/game/Card.hpp"
#include "client/game/Deck.hpp"
#include "client/game/Participant.hpp"
#include "client/game/SpriteSheet.hpp"
#include "client/game/Table.hpp"
#include "client/game/Types.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/InputEvents.hpp"
#include "shared/Types.hpp"

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

    std::vector<shared::game::Card*> cards;
    {
        auto pos = menu::Position{
            .m_position = sf::Vector2f(75.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        };

        unsigned short deckSize = shared::game::StandartDeckSize;
        cards.reserve(deckSize);
        for (unsigned short i = 0; i < deckSize; ++i)
        {
            auto cardPair = game::Card::getCardFromIndex(i);
            auto card = std::make_shared<game::Card>(getContext(), pos.calculateGlobalPos());
            card->setId(game::CardId(static_cast<uint8_t>(i)));
            getContainer(core::object::Container::Type::Game).add(card);
            cards.push_back(card.get());
        }
    }
    auto deck = std::make_shared<game::Deck>(
        std::move(cards), getContext().get<shared::Seed>().seed
    );
    getContainer(core::object::Container::Type::Game).add(deck);


    auto spawnPoints = table->generateSpawnPoints(playerManagerRef.getPlayers().size(), sf::Vector2f(windowRef.getSize()) / 2.f);
    
    std::vector<game::Participant*> participants;
    {
        size_t index = playerManagerRef.getIndexOfLocalPlayer();

        for (const auto& player : playerManagerRef.getPlayers())
        {
            auto playerId = player.id;
            unsigned short numberOfSlots = shared::game::MaxNumberOfParticipantSlots;
            std::vector<game::ParticipantSlot> slots;
            slots.reserve(numberOfSlots);
            for (game::ParticipantSlotId::Type slotId = 0; slotId < numberOfSlots; ++slotId)
            {
                // TODO make a choosable button?
                auto slotButton = std::make_shared<menu::item::Button>(
                    menu::Position{},
                    textureHolderRef.get(TextureIds::Cards),
                    game::spriteSheet::getDiscardTextureRect(),
                    game::spriteSheet::getDiscardTextureRect(game::spriteSheet::Hover::Yes),
                    [this, slotId, playerId](){
                        auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
                        eventDispatcherRef.send<events::LocalPlayerClickSlotEvent>(game::ParticipantSlotId(slotId), playerId);
                    },
                    sf::Mouse::Button::Left
                );
                slotButton->setActivationOption(core::object::Object::ActivationOption::Manually);
                
                getContainer(core::object::Container::Type::Menu).add(slotButton);

                slots.emplace_back(game::ParticipantSlot{ game::ParticipantSlotId(slotId), false, nullptr, *slotButton, {} });
            }
            
            auto participant = std::make_shared<game::Participant>(
                getContext(), playerId, true, std::move(slots), shared::game::DefaultInitNumberOfParticipantSlots
            );
            participant->setSpawnPoint(spawnPoints[index]);
            getContainer(core::object::Container::Type::Game).add(participant);
            
            participants.push_back(participant.get());

            index++;
            if (index >= spawnPoints.size())
                index = 0;
        }
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

    menu::Position discardPos = menu::Position{
        .m_position = sf::Vector2f(-75.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
        .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
    };
    auto discardButton = std::make_shared<menu::item::Button>(
        discardPos,
        textureHolderRef.get(TextureIds::Cards),
        game::spriteSheet::getDiscardTextureRect(),
        game::spriteSheet::getDiscardTextureRect(game::spriteSheet::Hover::Yes),
        [this, &eventDispatcherRef](){
            eventDispatcherRef.send<events::LocalPlayerClickDiscardEvent>();
        },
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(discardButton);

    auto finishButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(100.f, 100.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_END, .m_specPositionY = menu::Position::Special::OFFSET_FROM_END
        },
        textureHolderRef.get(TextureIds::FinishButton),
        game::spriteSheet::getFinishButtonTextureRect(),
        game::spriteSheet::getFinishButtonTextureRect(game::spriteSheet::Hover::Yes),
        [this, &eventDispatcherRef](){
            eventDispatcherRef.send<events::LocalPlayerClickFinishButtonEvent>();
        },
        sf::Mouse::Button::Left
    );
    finishButton->setActivationOption(core::object::Object::ActivationOption::Manually);
    getContainer(core::object::Container::Type::Menu).add(finishButton);

    auto caboButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(180.f, 100.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_END, .m_specPositionY = menu::Position::Special::OFFSET_FROM_END
        },
        textureHolderRef.get(TextureIds::CaboButton),
        game::spriteSheet::getCaboButtonTextureRect(),
        game::spriteSheet::getCaboButtonTextureRect(game::spriteSheet::Hover::Yes),
        [this, &eventDispatcherRef](){
            eventDispatcherRef.send<events::LocalPlayerClickCaboButtonEvent>();
        },
        sf::Mouse::Button::Left
    );
    caboButton->setActivationOption(core::object::Object::ActivationOption::Manually);
    getContainer(core::object::Container::Type::Menu).add(caboButton);

    game::Board::DecideActionButtons decideActionButtons;
    {
        auto matchButton = std::make_shared<menu::item::Button>(
            menu::Position{
                .m_position = sf::Vector2f(-150.f, 200.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
            },
            textureHolderRef.get(TextureIds::DecideButtons),
            game::spriteSheet::getMatchButtonTextureRect(),
            game::spriteSheet::getMatchButtonTextureRect(game::spriteSheet::Hover::Yes),
            [this, &eventDispatcherRef](){
                eventDispatcherRef.send<events::LocalPlayerClickDecideButtonEvent>(game::ActionType::Match);
            },
            sf::Mouse::Button::Left
        );
        matchButton->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(matchButton);
        decideActionButtons.push_back(matchButton.get());

        auto takeButton = std::make_shared<menu::item::Button>(
            menu::Position{
                .m_position = sf::Vector2f(-70.f, 200.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
            },
            textureHolderRef.get(TextureIds::DecideButtons),
            game::spriteSheet::getTakeButtonTextureRect(),
            game::spriteSheet::getTakeButtonTextureRect(game::spriteSheet::Hover::Yes),
            [this, &eventDispatcherRef](){
                eventDispatcherRef.send<events::LocalPlayerClickDecideButtonEvent>(game::ActionType::Take);
            },
            sf::Mouse::Button::Left
        );
        takeButton->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(takeButton);
        decideActionButtons.push_back(takeButton.get());

        auto actionButton = std::make_shared<menu::item::Button>(
            menu::Position{
                .m_position = sf::Vector2f(70.f, 200.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
            },
            textureHolderRef.get(TextureIds::DecideButtons),
            game::spriteSheet::getActionButtonTextureRect(),
            game::spriteSheet::getActionButtonTextureRect(game::spriteSheet::Hover::Yes),
            [this, &eventDispatcherRef](){
                eventDispatcherRef.send<events::LocalPlayerClickDecideButtonEvent>(game::ActionType::Ability);
            },
            sf::Mouse::Button::Left
        );
        actionButton->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(actionButton);
        decideActionButtons.push_back(actionButton.get());

        auto discardDecideButton = std::make_shared<menu::item::Button>(
            menu::Position{
                .m_position = sf::Vector2f(150.f, 200.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
            },
            textureHolderRef.get(TextureIds::DecideButtons),
            game::spriteSheet::getDiscardButtonTextureRect(),
            game::spriteSheet::getDiscardButtonTextureRect(game::spriteSheet::Hover::Yes),
            [this, &eventDispatcherRef](){
                eventDispatcherRef.send<events::LocalPlayerClickDecideButtonEvent>(game::ActionType::Discard);
            },
            sf::Mouse::Button::Left
        );
        discardDecideButton->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(discardDecideButton);
        decideActionButtons.push_back(discardDecideButton.get());
    }
    game::Board::DecideSwapButtons decideSwapButtons;
    {
        auto yesButton = std::make_shared<menu::item::Button>(
            menu::Position{
                .m_position = sf::Vector2f(-50.f, 250.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
            },
            textureHolderRef.get(TextureIds::YesNoButtons),
            game::spriteSheet::getYesButtonTextureRect(),
            game::spriteSheet::getYesButtonTextureRect(game::spriteSheet::Hover::Yes),
            [this, &eventDispatcherRef](){
                eventDispatcherRef.send<events::LocalPlayerClickDecideSwapButtonEvent>(true);
            },
            sf::Mouse::Button::Left
        );
        yesButton->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(yesButton);
        decideSwapButtons.push_back(yesButton.get());

        auto noButton = std::make_shared<menu::item::Button>(
            menu::Position{
                .m_position = sf::Vector2f(50.f, 250.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
            },
            textureHolderRef.get(TextureIds::YesNoButtons),
            game::spriteSheet::getNoButtonTextureRect(),
            game::spriteSheet::getNoButtonTextureRect(game::spriteSheet::Hover::Yes),
            [this, &eventDispatcherRef](){
                eventDispatcherRef.send<events::LocalPlayerClickDecideSwapButtonEvent>(false);
            },
            sf::Mouse::Button::Left
        );
        noButton->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(noButton);
        decideSwapButtons.push_back(noButton.get());
    }

    std::vector<menu::item::SimpleText*> texts;
    {
        constexpr unsigned NumberOfTextElements = 5;
        for (unsigned num = NumberOfTextElements; num > 0; --num)
        {
            auto text = std::make_shared<menu::item::SimpleText>(
                menu::Position{},
                "",
                fontHolderRef.get(FontIds::Main),
                24,
                sf::Color::White
            );
            text->setActivationOption(core::object::Object::ActivationOption::Manually);
            getContainer(core::object::Container::Type::Menu).add(text);
            texts.push_back(text.get());
        }
    }
    auto queue = std::make_shared<menu::item::NotificationQueue>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        sf::Time(sf::seconds(5.f)),
        50.f,
        texts
    );
    getContainer(core::object::Container::Type::Menu).add(queue);

    m_board = std::make_unique<game::Board>(
        getContext(), std::move(participants), *deck, *queue, *finishButton, *caboButton,
        std::move(decideActionButtons), std::move(decideSwapButtons), 
        game::CardPositions{ 
            .discardPos = discardPos.calculateGlobalPos(),
            .firstMatchPos = discardPos.calculateGlobalPos() + sf::Vector2f(-80.f, 0.f)
        }
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
