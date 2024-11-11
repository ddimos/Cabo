#include "states/MainMenuState.hpp"

#include "states/StateIds.hpp"
#include "ResourceIds.hpp"

#include "menu/item/Button.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::states
{

MainMenuState::MainMenuState(state::StateManager& _stateManagerRef)
    : State(_stateManagerRef)
{
    auto startButton = std::make_shared<menu::item::Button>(
        menu::Position{
             menu::Position::Special::CENTER_ALLIGNED, menu::Position::Special::OFFSET_FROM_CENTER, sf::Vector2f(0, -45.f)
        },
        getContext().textureHolderRef.get(TextureIds::MainMenuStartButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](bool _isPressed){
            pop();
            push(id::Game);
        },
        sf::Mouse::Button::Left,
        getContext().windowRef
    );
    getContainer().add(startButton);

    auto joinButton = std::make_shared<menu::item::Button>(
        menu::Position{
             menu::Position::Special::CENTER_ALLIGNED, menu::Position::Special::OFFSET_FROM_CENTER, sf::Vector2f(0, 45.f)
        },
        getContext().textureHolderRef.get(TextureIds::MainMenuJoinButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](bool _isPressed){

        },
        sf::Mouse::Button::Left,
        getContext().windowRef
    );
    
    getContainer().add(joinButton);
}

state::State::Return MainMenuState::onHandleEvent(const sf::Event& _event)
{
	return Return::Break;
}

state::State::Return MainMenuState::onUpdate(sf::Time _dt)
{
    return Return::Break;
}

void MainMenuState::onDraw()
{
}

} // namespace cn::states
