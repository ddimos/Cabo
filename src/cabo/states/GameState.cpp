#include "states/GameState.hpp"
#include "states/StateIds.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/SystemEvents.hpp"

#include "game/Card.hpp"
#include "game/Constants.hpp"
#include "game/Deck.hpp"
#include "game/Discard.hpp"
#include "game/Player.hpp"
#include "game/SpriteSheet.hpp"
#include "game/Table.hpp"

#include "menu/item/Button.hpp"
#include "menu/item/SimpleImage.hpp"
#include "menu/item/SimpleText.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <ctime>

#include "core/Log.hpp"

namespace cn::states
{

GameState::GameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);
    createContainer(core::object::Container::Type::Game);

    unsigned seed = static_cast<unsigned>(std::time(nullptr));

    std::vector<game::CardPtr> cards;
    {
        unsigned short deckSize = game::StandartDeckSize;
        cards.reserve(deckSize);
        for (unsigned short i = 0; i < deckSize; ++i)
        {
            auto cardPair = game::Card::getCardFromIndex(i);
            auto image = std::make_shared<menu::item::SimpleImage>(
                menu::Position{},
                getContext().textureHolderRef.get(TextureIds::Cards),
                game::spriteSheet::getCardTextureRect(cardPair.first, cardPair.second)
            );
            image->setActivationOption(core::object::Object::ActivationOption::Manually);
            getContainer(core::object::Container::Type::Menu).add(image);
            auto card = std::make_shared<game::Card>(cardPair.first, cardPair.second, image);
            card->setActivationOption(core::object::Object::ActivationOption::Manually);
            getContainer(core::object::Container::Type::Game).add(card);
            cards.push_back(card);
        }
    }

    auto deckButton = std::make_shared<menu::item::Button>(
        getContext().textureHolderRef.get(TextureIds::Cards),
        game::spriteSheet::getCardBackTextureRect(),
        game::spriteSheet::getCardBackTextureRect(game::spriteSheet::Hover::Yes),
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(deckButton);
    game::DeckPtr deck = std::make_shared<game::Deck>(
        deckButton, std::move(cards), seed
    );

    auto discardButton = std::make_shared<menu::item::Button>(
        getContext().textureHolderRef.get(TextureIds::Cards),
        game::spriteSheet::getDiscardTextureRect(),
        game::spriteSheet::getDiscardTextureRect(game::spriteSheet::Hover::Yes),
        sf::Mouse::Button::Left
    );
    discardButton->setActivationOption(core::object::Object::ActivationOption::Manually);

    getContainer(core::object::Container::Type::Menu).add(discardButton);
    game::DiscardPtr discard = std::make_shared<game::Discard>(
        discardButton
    );

    m_table = std::make_shared<game::Table>(
        getContext(), deck, discard
    );
    deckButton->setClickCallback([this](bool _pressed){
        m_table->onLocalPlayerClickDeck();
    });
    discardButton->setClickCallback([this](bool _pressed)
    {
        m_table->onLocalPlayerClickDiscard();
    });

    getContainer(core::object::Container::Type::Game).add(m_table);
    getContainer(core::object::Container::Type::Game).add(deck);
    getContainer(core::object::Container::Type::Game).add(discard);

    m_listenerId = core::event::getNewListenerId();
}

void GameState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::KeyReleasedEvent>(m_listenerId,
            [this](const events::KeyReleasedEvent& _event){
                if (_event.key.code == sf::Keyboard::Space)
                {
                    pop();
                    push(id::Finish);
                }
                if (_event.key.code == sf::Keyboard::P)
                {
                    std::vector<game::PlayerSlot> slots;

                    unsigned short numberOfSlots = game::MaxNumberOfPlayerSlots;
                    slots.reserve(numberOfSlots);
                    for (game::PlayerSlotId i = 0; i < numberOfSlots; ++i)
                    {
                        // TODO make a choosable button?
                        auto slotButton = std::make_shared<menu::item::Button>(
                            getContext().textureHolderRef.get(TextureIds::Cards),
                            game::spriteSheet::getCardBackTextureRect(),
                            game::spriteSheet::getCardBackTextureRect(game::spriteSheet::Hover::Yes),
                            sf::Mouse::Button::Left
                        );
                        slotButton->setActivationOption(core::object::Object::ActivationOption::Manually);
                        getContainer(core::object::Container::Type::Menu).add(slotButton);
                        slots.emplace_back(game::PlayerSlot{ .id = i, .m_button = slotButton });
                    }
                    auto player = std::make_shared<game::Player>(
                        getContext(), std::move(slots), game::DefaultInitNumberOfPlayerSlots
                    );
                    getContainer(core::object::Container::Type::Game).add(player);

                    m_table->addPlayer(player);
                    player->requestActivated();
                }
                if (_event.key.code == sf::Keyboard::S)
                {
                    m_table->start();
                }
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::KeyReleasedEvent>(m_listenerId);
    }

    // m_gameContainer.registerEvents(_dispatcher, _isBeingRegistered);
}

} // namespace cn::states
