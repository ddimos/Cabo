#include "states/GameState.hpp"
#include "states/StateIds.hpp"

#include "game/Constants.hpp"
#include "game/Deck.hpp"
#include "game/Discard.hpp"
#include "game/Player.hpp"
#include "game/Table.hpp"

#include "menu/item/SimpleImage.hpp"
#include "menu/item/SimpleText.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <ctime>

namespace cn::states
{

GameState::GameState(state::StateManager& _stateManagerRef)
    : State(_stateManagerRef)
{
    auto text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(getContext().windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Play",
        getContext().fontHolderRef.get(FontIds::Main),
        20,
        sf::Color::White
    );
    getMenuContainer().add(text);

    unsigned seed = static_cast<unsigned>(std::time(nullptr));

    game::DeckPtr deck = std::make_shared<game::Deck>(
        getContext(), game::StandartDeckSize, seed
    );

    game::DiscardPtr discard = std::make_shared<game::Discard>(
        getContext()
    );

    m_table = std::make_shared<game::Table>(
        getContext(), deck, discard, std::time(nullptr)
    );

    getGameContainer().add(m_table);
    getGameContainer().add(deck);
    getGameContainer().add(discard);
}

state::State::Return GameState::onHandleEvent(const sf::Event& _event)
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
            auto player = std::make_shared<game::Player>(
                getContext()
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
    return Return::Break;
}

state::State::Return GameState::onUpdate(sf::Time _dt)
{
    m_gameContainer.update(_dt);

    return Return::Break;
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
