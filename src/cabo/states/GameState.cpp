#include "states/GameState.hpp"
#include "states/StateIds.hpp"

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

namespace cn::states
{

GameState::GameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
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
            getMenuContainer().add(image);
            auto card = std::make_shared<game::Card>(cardPair.first, cardPair.second, image);
            card->setActivationOption(core::object::Object::ActivationOption::Manually);
            getGameContainer().add(card);
            cards.push_back(card);
        }
    }

    auto deckButton = std::make_shared<menu::item::Button>(
        getContext().textureHolderRef.get(TextureIds::Cards),
        game::spriteSheet::getCardBackTextureRect(),
        game::spriteSheet::getCardBackTextureRect(game::spriteSheet::Hover::Yes),
        sf::Mouse::Button::Left
    );
    getMenuContainer().add(deckButton);
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

    getMenuContainer().add(discardButton);
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


    getGameContainer().add(m_table);
    getGameContainer().add(deck);
    getGameContainer().add(discard);
}

core::state::Return GameState::onHandleEvent(const sf::Event& _event)
{
    m_gameContainer.handleEvent(_event);

    if (_event.type == sf::Event::KeyReleased)
    {
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
                getMenuContainer().add(slotButton);
                slots.emplace_back(game::PlayerSlot{ .id = i, .m_button = slotButton });
            }
            auto player = std::make_shared<game::Player>(
                getContext(), std::move(slots), game::DefaultInitNumberOfPlayerSlots
            );
            getGameContainer().add(player);

            m_table->addPlayer(player);
            player->activate();
        }
        if (_event.key.code == sf::Keyboard::S)
        {
            m_table->start();
        }
    }
    return core::state::Return::Break;
}

core::state::Return GameState::onUpdate(sf::Time _dt)
{
    m_gameContainer.update(_dt);

    return core::state::Return::Break;
}

void GameState::onDraw()
{
    m_gameContainer.draw(getContext().windowRef);
}

void GameState::onActivate()
{
    m_gameContainer.activate();
}

void GameState::onDeactivate()
{
    m_gameContainer.deactivate();
}

} // namespace cn::states
